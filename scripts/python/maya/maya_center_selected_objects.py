import maya.cmds as mc

# Note (trent, 5/7/18): This script assumes a Z-up scene (and that all selected objects should be placed above the origin on Z with their pivots at the origin. 
def moveToCenter(sel, center = True):
   if len(sel)<=0:
       print "select one object!"
   if center:
       mc.xform (sel, centerPivots = center)
   else:
       bbox = mc.exactWorldBoundingBox(sel)
       bottom = [(bbox[0] + bbox[3])/2, (bbox[1] + bbox[4])/2, bbox[2]]
       mc.xform (sel, piv=bottom, ws=True)
       mc.move(0,0,0, sel, rotatePivotRelative = True)

selection = mc.ls(sl=True)
for sel in selection:
    moveToCenter(sel, center = False)