#BaseStationGUI.py

import mraa
import time
import sys
import math
import os
import paho.mqtt.client as mqtt

#Uncomment if you want to show when it connects to MQTT
def on_connect(client, userdata, flags, rc):
    broker_client.subscribe('oor', 0)
    print("rc: " + str(rc))

#When subscribed to a topic
def on_subscribe(client, obj, mid, granted_qos):
    print("Subscribed: " +str(mid_ + " " + str(granted_qos)))

#When a message is received from the broker do something
def on_message(client, obj, msg):
    print("Message received from topic: " + msg.topic)
    display = Curr_Dir + str(msg.payload)
    os.system(display)
    time.sleep(3)

    #global declarations
    global Men_Sel
    global Men_Dist
    global Men_Act
    global Men_Col
    global Sel_Ind
    global Col_Ind
    global Act_Ind
    global Dist_ind
    #Reset all index and go back to bracelet menu
    Men_Sel   = 1
    Men_Dist  = 0
    Men_Act   = 0
    Men_Col   = 0
    Sel_Ind   = 0
    Col_Ind   = 0
    Act_Ind   = 0
    Dist_Ind  = 0
    display = Curr_Dir + Bracelet_Menu[Sel_Ind]
    os.system(display)
    
#When a message is published do something
def on_publish(client,userdata,result):
    print("data published \n")
    pass


#MQTT
mqtt_broker = "192.168.42.1"
mqtt_port = 1883
mqtt_username = "bracelet1"
mqtt_password = "12345"

broker_client = mqtt.Client()
broker_client.on_message = on_message
broker_client.on_publish = on_publish
broker_client.on_connect = on_connect
broker_client.on_subscribe = on_subscribe

broker_client.username_pw_set(mqtt_username, mqtt_password)
broker_client.connect(mqtt_broker,mqtt_port)
broker_client.subscribe('oor', 0)
broker_client.loop_start()
#Current Directory
Curr_Dir = "/home/root/GUI/lcddisplay "

#MENU
Bracelet_Menu = ['Bracelet1', 'Bracelet2', 'Bracelet3', 'Bracelet4', 'Bracelet5']
Action_Menu = ['Color', 'Distance']
Color_Menu = ['Blue', 'Green', 'Orange', 'Yellow', 'Pink', 'Purple']
Distance_Menu = ['3', '5', '10', '15', '20', '25', '30', '35', '40', '45', '50']

#U3800A Buttons
#Button_UP: Button1 - GPIO 33
#Button_DN: Button2 - GPIO 46
#Button_EN: Button3 - GPIO 32
#Button_BK: Button4 - GPIO 45
Button_UP = mraa.Gpio(33)
Button_UP.dir(mraa.DIR_IN)

Button_DN = mraa.Gpio(46)
Button_DN.dir(mraa.DIR_IN)

Button_EN = mraa.Gpio(32)
Button_EN.dir(mraa.DIR_IN)

Button_BK = mraa.Gpio(45)
Button_BK.dir(mraa.DIR_IN)

#Selection Variables
Bracelet_Sel = Bracelet_Menu[0]
Bracelet_Act = Action_Menu[0]
Bracelet_Col = Color_Menu[0]
Bracelet_Dist = Distance_Menu[0]

#Menu Index
Sel_Ind = 0 #0-4
Act_Ind = 0 #0-1
Col_Ind = 0 #0-5
Dist_Ind = 0 #0-10

#Menu Check
Men_Sel = 1
Men_Act = 0
Men_Col = 0
Men_Dist = 0

display = Curr_Dir + Bracelet_Menu[Sel_Ind]
os.system(display)

while(1):

    #Braclet Menu
    time.sleep(0.5)
    #if in Select Menu
    if Men_Sel == 1:
        #Detect UP Press
        if Button_UP.read() == 0:
            if Sel_Ind == 4:
                Sel_Ind = 0
            else:
                Sel_Ind = Sel_Ind + 1
                
            #Display Next Menu Choice
            display = Curr_Dir + Bracelet_Menu[Sel_Ind]
            os.system(display)
        #Detect DOWN Press    
        if Button_DN.read() == 0:
            if Sel_Ind == 0:
                Sel_Ind = 4
            else:
                Sel_Ind = Sel_Ind - 1
                
            #Display Prev Menu Choice
            display = Curr_Dir + Bracelet_Menu[Sel_Ind]
            os.system(display)
            
        #Detect ENTER Press    
        if Button_EN.read() == 0:
            #Bracelet Selected, Enter & Display Action Menu
            Men_Act  = 1
            display = Curr_Dir + Action_Menu[Act_Ind]
            os.system(display)

            #Not in any other menu
            Men_Sel  = 0
            Men_Col  = 0
            Men_Dist = 0
            
    
    #if in Action Menu
    elif Men_Act == 1:
        
        #Detect UP Press
        if Button_UP.read() == 0:
            if Act_Ind == 1:
                Act_Ind = 0
            else:
                Act_Ind = Act_Ind + 1
                
            #Display Next Menu Choice
            display = Curr_Dir + Action_Menu[Act_Ind]
            os.system(display)
        #Detect DOWN Press    
        if Button_DN.read() == 0:
            if Act_Ind == 0:
                Act_Ind = 1
            else:
                Act_Ind = Act_Ind - 1
                
            #Display Prev Menu Choice
            display = Curr_Dir + Action_Menu[Act_Ind]
            os.system(display)
            
        #Detect ENTER Press    
        if Button_EN.read() == 0:
            #Action Selected
            #if Action Index == 0, Go to Color Menu
            if Act_Ind == 0:
                Men_Col = 1
                display = Curr_Dir + Color_Menu[Col_Ind]
                os.system(display)

                #Not in any other menu
                Men_Sel  = 0
                Men_Act  = 0
                Men_Dist = 0
            #else Go to Distance Menu
            else:
                Men_Dist = 1
                display = Curr_Dir + Distance_Menu[Dist_Ind]
                os.system(display)

                #Not in any other menu
                Men_Sel  = 0
                Men_Act  = 0
                Men_Col  = 0
                
            
        #Detect BACK Press    
        if Button_BK.read() == 0:
            #Reset Action Index
            Act_Ind = 0;

            #Enter Bracelet Menu
            Men_Sel = 1
            display = Curr_Dir + Bracelet_Menu[Sel_Ind]
            os.system(display)
            
            #Not in any other menu
            Men_Dist  = 0
            Men_Act   = 0
            Men_Col   = 0

    #if in Color_Menu
    elif Men_Col == 1:
        
        #Detect UP Press
        if Button_UP.read() == 0:
            if Col_Ind == 5:
                Col_Ind = 0
            else:
                Col_Ind = Col_Ind + 1
                
            #Display Next Menu Choice
            display = Curr_Dir + Color_Menu[Col_Ind]
            os.system(display)
        #Detect DOWN Press    
        if Button_DN.read() == 0:
            if Col_Ind == 0:
                Col_Ind = 5
            else:
                Col_Ind = Col_Ind - 1
                
            #Display Prev Menu Choice
            display = Curr_Dir + Color_Menu[Col_Ind]
            os.system(display)
            
        #Detect ENTER Press    
        if Button_EN.read() == 0:
            #Change Bracelet Color via MQTT using Sel_Ind & Col_Ind **TODO**
            topic = Bracelet_Menu[Sel_Ind] + "/" + "Color"
            broker_client.publish(topic,Color_Menu[Col_Ind])
            #Display the selection
            message = Bracelet_Menu[Sel_Ind] + '_' + Color_Menu[Col_Ind]
            display = Curr_Dir + message
            os.system(display)
            time.sleep(3)

            #Reset all index and go back to bracelet menu
            Men_Sel   = 1
            Men_Dist  = 0
            Men_Act   = 0
            Men_Col   = 0

            Sel_Ind   = 0
            Col_Ind   = 0
            Act_Ind   = 0
            Dist_Ind  = 0
            
            display = Curr_Dir + Bracelet_Menu[Sel_Ind]
            os.system(display)
            
        #Detect BACK Press    
        if Button_BK.read() == 0:
            #Reset Color Index
            Col_Ind = 0;

            #Enter Action Menu
            Men_Act = 1
            display = Curr_Dir + Action_Menu[Act_Ind]
            os.system(display)
            
            #Not in any other menu
            Men_Dist  = 0
            Men_Sel   = 0
            Men_Col   = 0

    #if in Distance Menu
    elif Men_Dist == 1:
        
        #Detect UP Press
        if Button_UP.read() == 0:
            if Dist_Ind == 10:
                Dist_Ind = 0
            else:
                Dist_Ind = Dist_Ind + 1
                
            #Display Next Menu Choice
            display = Curr_Dir + Distance_Menu[Dist_Ind]
            os.system(display)
        #Detect DOWN Press    
        if Button_DN.read() == 0:
            if Dist_Ind == 0:
                Dist_Ind = 10
            else:
                Dist_Ind = Dist_Ind - 1
                
            #Display Prev Menu Choice
            display = Curr_Dir + Distance_Menu[Dist_Ind]
            os.system(display)
            
        #Detect ENTER Press    
        if Button_EN.read() == 0:
            #Change Bracelet Distance via MQTT using Sel_Ind & Dist_Ind **TODO**
            topic = Bracelet_Menu[Sel_Ind] + "/" + "Distance"
            broker_client.publish(topic,Distance_Menu[Dist_Ind])
            
            #Display the selection
            message = Bracelet_Menu[Sel_Ind] + '_' + Distance_Menu[Col_Ind]
            display = Curr_Dir + message
            os.system(display)
            time.sleep(3)

            #Reset all index and go back to bracelet menu
            Men_Sel   = 1
            Men_Dist  = 0
            Men_Act   = 0
            Men_Col   = 0

            Sel_Ind   = 0
            Col_Ind   = 0
            Act_Ind   = 0
            Dist_Ind  = 0
            
            display = Curr_Dir + Bracelet_Menu[Sel_Ind]
            os.system(display)
            
        #Detect BACK Press    
        if Button_BK.read() == 0:
            #Reset Distance Index
            Dist_Ind = 0;

            #Enter Action Menu
            Men_Act = 1
            display = Curr_Dir + Action_Menu[Act_Ind]
            os.system(display)
            
            #Not in any other menu
            Men_Dist  = 0
            Men_Sel   = 0
            Men_Col   = 0
