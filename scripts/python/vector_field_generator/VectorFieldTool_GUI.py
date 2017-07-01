#!/usr/bin/python

# Author: DeepSpaceBanana, VectorFieldScript by Kashaar(Jan Kaluza)
# Date:1/20/17

from Tkinter import *
from tkFileDialog import *
from tkMessageBox import *
from ttk import Combobox
from math import ceil
import os
import VFGenerator as core
from tooltip import CreateToolTip



# Define Basic Background and Text Colors
textcol = "#999999"
bgColor = "#282828"
GenVar = IntVar
biasVar = IntVar
b_height = 20
b_width = 20
b_xpad = b_width + 20

def assignValues():
    gridsize = [int(ceil(float(grids_x.get()))),int(ceil(float(grids_y.get()))),int(ceil(float(grids_z.get())))]
    generator = getGenType(GenType_dpd.get())
    minbounds = [int(MinB_x.get()),int(MinB_y.get()),int(MinB_z.get())]
    maxbounds = [int(MaxB_x.get()),int(MaxB_y.get()),int(MaxB_z.get())]
    directionbias = [float(DirB_x.get()),float(DirB_y.get()),float(DirB_z.get())]
    directionstrength = float(DirStr.get())
    scalenoiseamount = float(SclNAmt.get())
    directionnoiseamount = float(DirNAmt.get())
    mainscalefactor = float(Scl_F.get())
    filename = File_Entry.get()

    filecheck = os.path.isfile(filename)
    pathcheck = os.path.split(filename)
    pathbool = os.path.isdir(pathcheck[0])

    if(pathbool):
        core.makeVectorField(gridsize, minbounds, maxbounds, generator, filename, directionbias, directionstrength,scalenoiseamount, directionnoiseamount, mainscalefactor)
    else:
        showerror("Save Location", "Invalid save location")

    #print(gridsize)
    #print(generator)
    #print(File_Entry.get())
    #print(minbounds)
    #print(maxbounds)
    #print(directionbias)
    #print(directionstrength)
    #print(scalenoiseamount)
    #print(directionnoiseamount)
    #print(mainscalefactor)

def getGenType(string01):
    return{"Uniform":0,"UniformNormalized":1,"Grid":2,"GridNormalized":3,"Radial":4}[string01]

def saveTo():
    File_Entry.delete(0,'end')
    saveDir = asksaveasfilename()
    saveDir = "%s.fga" % saveDir
    File_Entry.insert(2,str(saveDir))

def updatedirstr(event):
    DirStr.delete(0,'end')
    DirStr.insert(1,str(DirStr_slider.get()))

def updatescalenoise(event):
    SclNAmt.delete(0,'end')
    SclNAmt.insert(1,str(SclNAmt_slider.get()))

def updatedirnamt(event):
    DirNAmt.delete(0,'end')
    DirNAmt.insert(1,str(DirNAmt_slider.get()))

def updatemainscl(event):
    Scl_F.delete(0,'end')
    Scl_F.insert(3,str(Scl_F_slider.get()))


GenTypeOptions = ["Uniform","UniformNormalized","Grid","GridNormalized","Radial"]
dirbiasoptions = [-1,0,1]

root = Tk()
root.option_add("*TCombobox*Listbox*Background", '#282828')
root.option_add("*TCombobox*Listbox*Foreground", '#999999')
root.title("Kashaar's VectorField Thingymabub v1.0")
root.geometry("400x512")
root.configure(background=bgColor)
root.resizable(width=False, height=False)

# Create Generator Type Dropdown Widgets
GenType_lbl = Label(root,text="Choose Generator Type: ",fg=textcol,bg=bgColor)
GenType_lbl.place(x=20,y=20,height=20)
GenType_tlt = CreateToolTip(GenType_lbl,"Choose what type of noise generator to use")

GenType_dpd = Combobox(root,values=GenTypeOptions,textvariable=GenVar,takefocus=True,state="readonly")
GenType_dpd.set(GenTypeOptions[3])
GenType_dpd.place(x=170,y=20,height=20)



#Create Gridsize Entry Widget

Grid_lbl = Label(root,text="GridSize:",fg=textcol,bg=bgColor)
Grid_lbl.place(x=20,y=50,height=20)

Grid_tlt = CreateToolTip(Grid_lbl,"Use values no lower than 2. High values take longer to generate, and cost more memory.")

gridx_lbl = Label(root,text="X:",fg=textcol,bg=bgColor)
gridx_lbl.place(x=80,y=50,height=20,width=40)
grids_x = Entry(root,bg=bgColor,fg=textcol)
grids_x.insert(2,"16")
grids_x.place(x=120,y=50,height=20,width=40)

gridy_lbl = Label(root,text="Y:",fg=textcol,bg=bgColor)
gridy_lbl.place(x=160,y=50,height=20,width=40)
grids_y = Entry(root,bg=bgColor,fg=textcol)
grids_y.insert(2,"16")
grids_y.place(x=200,y=50,height=20,width=40)

gridz_lbl = Label(root,text="Z:",fg=textcol,bg=bgColor)
gridz_lbl.place(x=240,y=50,height=20,width=40)
grids_z = Entry(root,bg=bgColor,fg=textcol)
grids_z.insert(2,"16")
grids_z.place(x=280,y=50,height=20,width=40)

# Create MinBounds Entry Widget

MinB_lbl = Label(root,text="MinBounds:",fg=textcol,bg=bgColor)
MinB_lbl.place(x=20,y=80,height=20)
MinB_tlt = CreateToolTip(MinB_lbl,"bottom left back coordinate of the grid")

MinBx_lbl = Label(root,text="X:",fg=textcol,bg=bgColor)
MinBx_lbl.place(x=80,y=80,height=20,width=40)
MinB_x = Entry(root,bg=bgColor,fg=textcol)
MinB_x.insert(2,"-100")
MinB_x.place(x=120,y=80,height=20,width=40)

MinBy_lbl = Label(root,text="Y:",fg=textcol,bg=bgColor)
MinBy_lbl.place(x=160,y=80,height=20,width=40)
MinB_y = Entry(root,bg=bgColor,fg=textcol)
MinB_y.insert(2,"-100")
MinB_y.place(x=200,y=80,height=20,width=40)

MinBz_lbl = Label(root,text="Z:",fg=textcol,bg=bgColor)
MinBz_lbl.place(x=240,y=80,height=20,width=40)
MinB_z = Entry(root,bg=bgColor,fg=textcol)
MinB_z.insert(2,"-100")
MinB_z.place(x=280,y=80,height=20,width=40)

# Create MaxBounds Entry Widget

MaxB_lbl = Label(root,text="MaxBounds:",fg=textcol,bg=bgColor)
MaxB_lbl.place(x=20,y=110,height=20)

MaxB_tlt = CreateToolTip(MaxB_lbl,"top right front coordinate of the grid")

MaxBx_lbl = Label(root,text="X:",fg=textcol,bg=bgColor)
MaxBx_lbl.place(x=80,y=110,height=20,width=40)
MaxB_x = Entry(root,bg=bgColor,fg=textcol)
MaxB_x.insert(2,"100")
MaxB_x.place(x=120,y=110,height=20,width=40)

MaxBy_lbl = Label(root,text="Y:",fg=textcol,bg=bgColor)
MaxBy_lbl.place(x=160,y=110,height=20,width=40)
MaxB_y = Entry(root,bg=bgColor,fg=textcol)
MaxB_y.insert(2,"100")
MaxB_y.place(x=200,y=110,height=20,width=40)

MaxBz_lbl = Label(root,text="Z:",fg=textcol,bg=bgColor)
MaxBz_lbl.place(x=240,y=110,height=20,width=40)
MaxB_z = Entry(root,bg=bgColor,fg=textcol)
MaxB_z.insert(2,"100")
MaxB_z.place(x=280,y=110,height=20,width=40)

# Create DirectionBias Entry Widget

DirB_lbl = Label(root,text="Direction Bias:",fg=textcol,bg=bgColor)
DirB_lbl.place(x=20,y=140,height=20)

DirB_tlt = CreateToolTip(DirB_lbl,"Stick to values between -1 and 1.To make vector fields generally point in a specific direction, define the direction here...")

DirBx_lbl = Label(root,text="X:",fg=textcol,bg=bgColor)
DirBx_lbl.place(x=100,y=140,height=20,width=b_width)

#DirBx_dpd = Combobox(root,values=dirbiasoptions,takefocus=True,state="readonly")
#DirBx_dpd.set(dirbiasoptions[1])
#DirBx_dpd.place(x=120,y=140,height=20,width=40)

DirB_x = Entry(root,bg=bgColor,fg=textcol)
DirB_x.insert(2,"0")
DirB_x.place(x=120,y=140,height=20,width=b_width)

DirBy_lbl = Label(root,text="Y:",fg=textcol,bg=bgColor)
DirBy_lbl.place(x=180,y=140,height=20,width=b_width)

#DirBy_dpd = Combobox(root,values=dirbiasoptions,takefocus=True,state="readonly")
#DirBy_dpd.set(dirbiasoptions[1])
#DirBy_dpd.place(x=200,y=140,height=20,width=40)

DirB_y = Entry(root,bg=bgColor,fg=textcol)
DirB_y.insert(2,"0")
DirB_y.place(x=200,y=140,height=20,width=b_width)

DirBz_lbl = Label(root,text="Z:",fg=textcol,bg=bgColor)
DirBz_lbl.place(x=260,y=140,height=20,width=b_width)

#DirBz_dpd = Combobox(root,values=dirbiasoptions,takefocus=True,state="readonly")
#DirBz_dpd.set(dirbiasoptions[1])
#DirBz_dpd.place(x=280,y=140,height=20,width=40)

DirB_z = Entry(root,bg=bgColor,fg=textcol)
DirB_z.insert(2,"0")
DirB_z.place(x=280,y=140,height=20,width=b_width)

# Create DirectionStrength Entry Widget

DirStr_lbl = Label(root,text="Direction Strength:",fg=textcol,bg=bgColor)
DirStr_lbl.place(x=20,y=170,height=20)

DirStr_tlt = CreateToolTip(DirStr_lbl,"Use this parameter to scale DirectionBias.")

DirStr = Entry(root,bg=bgColor,fg=textcol)
DirStr.insert(4,"0.0")
DirStr.place(x=140,y=170,height=20,width=40)

DirStr_slider = Scale(root,from_=0,to=100,orient=HORIZONTAL,length=180,showvalue=0,resolution=-1,command=updatedirstr)
DirStr_slider.place(x=200,y=170)

# Create ScaleNoiseAmount Entry Widget

SclNAmt_lbl = Label(root,text="Scale Noise Amount:",fg=textcol,bg=bgColor)
SclNAmt_lbl.place(x=20,y=200,height=20)

SclNAmt_tlt = CreateToolTip(SclNAmt_lbl,"This is used to give some variation to the generated vectors' lengths. A factor of 1.0 for example would mean that instead of length 1, output vectors get a random length between 0 and 2.")

SclNAmt = Entry(root,bg=bgColor,fg=textcol)
SclNAmt.insert(3,"0.05")
SclNAmt.place(x=140,y=200,height=20,width=40)

SclNAmt_slider = Scale(root,from_=0,to=10,orient=HORIZONTAL,length=180,showvalue=0,resolution=-1,command=updatescalenoise)
SclNAmt_slider.place(x=200,y=200)
SclNAmt_slider.set(0.05)

# Create Widget for Direction Noise

DirNAmt_lbl = Label(root,text="Direction Noise Scale Amount:",fg=textcol,bg=bgColor)
DirNAmt_lbl.place(x=20,y=230,height=20)

DirNAmt_tlt = CreateToolTip(DirNAmt_lbl,"Adds uniform unidirectional noise to the generator's result, for nicer swirlies.")

DirNAmt = Entry(root,bg=bgColor,fg=textcol)
DirNAmt.insert(3,"0.0")
DirNAmt.place(x=200,y=230,height=20,width=40)

DirNAmt_slider = Scale(root,from_=0,to=10,orient=HORIZONTAL,length=120,showvalue=0,resolution=-1,command=updatedirnamt)
DirNAmt_slider.place(x=260,y=230)

# Create mainscalefactor Entry Widget

Scl_F_lbl = Label(root,text="Main Scale Factor",fg=textcol,bg=bgColor)
Scl_F_lbl.place(x=20,y=260,height=20)

Scl_F_tlt = CreateToolTip(Scl_F_lbl,"Generated vectors are (or should be) within 0 - 1 units of length. This variable lets you scale them all uniformly.")

Scl_F = Entry(root,bg=bgColor,fg=textcol)
Scl_F.insert(3,"10.0")
Scl_F.place(x=140,y=260,height=20,width=b_width*2)

Scl_F_slider = Scale(root,from_=-100,to=100,orient=HORIZONTAL,length=180,showvalue=0,resolution=-1,command=updatemainscl)
Scl_F_slider.place(x=200,y=260)
Scl_F_slider.set(10)

# Create File Dialog

File_lbl = Label(root,text="Save To",fg=textcol,bg=bgColor)
File_lbl.place(x=20,y=290,height=20)

File_tlt = CreateToolTip(File_lbl,"Path to save generated .fga file")

File_Entry = Entry(root,fg=textcol,bg=bgColor)
File_Entry.place(x=80,y=290,height=20,width=300)
#File_Entry.bind("<KeyRelease>",updatefileentry)

File_btn = Button(root,text="...",fg=textcol,bg=bgColor,command=saveTo)
File_btn.place(x=370,y=290,height=20)

Create_btn = Button(root,text="Generate VectorField",bg=bgColor,fg=textcol,command=assignValues)
Create_btn.place(x=128,y=340)

crtbtn_tlt = CreateToolTip(Create_btn,"Script will attempt to open the generated .fga file. Use any text editor.")

root.mainloop()

