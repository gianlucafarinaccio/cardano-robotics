# Soccerbot - By: frankie - gio mag 9 2019
# [] list
# () tupla

from math import atan2
from math import degrees
from math import trunc
from math import sqrt
import sensor, image, time, pyb

red_led = pyb.LED(1)
green_led = pyb.LED(2)
blue_led = pyb.LED(3)

red_led.off()
green_led.off()
blue_led.off()

uart = pyb.UART(3, 115200, timeout_char = 0)

def zfill(s, width):
    if len(s) < width:
        return ("0" * (width - len(s))) + s
    else:
        return s

# CAMERA SETTINGS
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
sensor.set_contrast(3)
sensor.set_saturation(+3)
sensor.set_brightness(0)
sensor.set_quality(0)
sensor.set_auto_exposure(False, 3500)
#sensor.set_auto_exposure(True)
sensor.set_auto_gain(True)
#sensor.set_auto_whitebal(False, (-6.02073, -5.753914, -1.557143))
sensor.set_hmirror(False) # horizontally
sensor.set_vflip(True) # vertically
sensor.skip_frames(time = 300)

frame_mode = 0

# THRESHOLDS
#(24, 42, 41, 63, 23, 48)
thresholds = [(46, 60, 57, 86, 22, 84),  # ball
              (46, 58, -20, 2, -48, -26)]  # goal

#X_COORD = 158
#Y_COORD = 122

X_COORD = 76
Y_COORD = 63

corn_goal_x = 0
corn_goal_y = 0

# FPS
clock = time.clock()

while(True) :
    clock.tick()

    # cx,cy,radius,code
    tt_ball = [(X_COORD+10,Y_COORD,0,1)] # lista di tuple palla
    # cx,cy,w,h,code
    tt_goal = [(X_COORD+10,Y_COORD,0,0,2)] # lista di tuple porta

    frame = sensor.snapshot()

    for blob in frame.find_blobs(thresholds) :
        if blob.code() == 1 :
            if blob.roundness() > 0.0:
                tt_ball = tt_ball + [(blob.cx(),blob.cy(),(blob.w()/2),blob.code())]

        if blob.code() == 2 :
            tt_goal = tt_goal + [(blob.cx(),blob.cy(),blob.w(),blob.h(),blob.code())]

    tt_ball.sort(key=lambda tup: tup[2])  ## ordino le liste
    tt_goal.sort(key=lambda tup: tup[2])    ## ordino le liste
    nb = len(tt_ball) #USARE QUESTI PER CAPIRE SE PALLA O NO
    ng = len(tt_goal) #LORO

    ball_cx,ball_cy,ball_radius,ball_code = tt_ball[nb-1]
    goal_cx,goal_cy,goal_w,goal_h,goal_code = tt_goal[ng-1]
    corn_goal_x = int(goal_cx)
    corn_goal_y = int(goal_cy+(goal_h/2))
    corn_goal_x2 = int(goal_cx+(goal_w/2))
    corn_goal_y2 = int(goal_cy-(goal_h/2))
    frame.draw_circle(corn_goal_x,corn_goal_y,2,(208,37,219),1,fill=False)
    frame.draw_circle(corn_goal_x,corn_goal_y2,2,(208,37,219),1,fill=False)
    frame.draw_line(80,60,corn_goal_x,corn_goal_y,(255,0,0),1)
    frame.draw_line(80,60,corn_goal_x,corn_goal_y2,(255,0,0),1)
    angolo = atan2(int(goal_h/2),int(goal_w))
    print("angolo=",angolo*2)

    # BALL COORD
    ball_seno = ball_cy - Y_COORD # segment "b", seno
    ball_coseno = ball_cx - X_COORD # segment "c", coseno
    ball_arctan = atan2(ball_seno, ball_coseno) # arctan
    ball_angle = degrees(ball_arctan) # convert in degrees
    ball_angle = trunc(ball_angle)
    if (ball_angle < 0) :
        ball_angle = ball_angle + 359
    ball_distance = sqrt(((ball_cx - X_COORD)**2) + ((ball_cy - Y_COORD)**2)) #214,121
    ball_distance = int(ball_distance)

    # GOAL COORD
    goal_seno = goal_cy - Y_COORD # segment "b", seno
    goal_coseno = goal_cx - X_COORD # segment "c", coseno
    goal_arctan = atan2(goal_seno, goal_coseno) # arctan
    goal_angle = degrees(goal_arctan) # convert in degrees
    goal_angle = trunc(goal_angle)
    if (goal_angle < 0) :
        goal_angle = goal_angle + 359
    goal_distance = sqrt(((goal_cx - X_COORD)**2) + ((goal_cy - Y_COORD)**2)) #214,121
    goal_distance = int(goal_distance)

    if nb == 1 :
        ball_angle = 999
        ball_distance = 999
        green_led.off()
    else :
        green_led.on()
    if ng == 1 :
        goal_angle = 999
        goal_distance = 999

    string_send = zfill(str(ball_angle),3)+zfill(str(ball_distance),3)+zfill(str(goal_angle),3)+zfill(str(goal_distance),3)+"\r"
    uart.write(string_send)

    if(frame_mode == 1):
        #frame.draw_line(80,0,80,120,(255,0,0),1)
        #frame.draw_line(0,60,160,60,(255,0,0),1)
        frame.draw_cross(ball_cx,ball_cy,(208,37,219))
        frame.draw_circle(ball_cx,ball_cy,int(ball_radius),(208,37,219),2,fill=False)
        frame.draw_cross(goal_cx,goal_cy,(0,255,0))
        frame.draw_rectangle((goal_cx-int(goal_w/2)),(goal_cy-int(goal_h/2)),goal_w,goal_h,(0,255,0),2,fill=False)

        print(trunc(clock.fps()))
        print("BA: ",ball_angle)
        print("GA: ",goal_angle)
        print("BD:",ball_distance)
        print(string_send)
        print("\n")
