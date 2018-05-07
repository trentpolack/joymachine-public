import maya.cmds as cmds

# Note (trent, 5/7/18): This script assumes a Z-up scene (and that all selected objects should be placed above the origin on Z with their pivots at the origin. 
def moveToCenter(sel):
   if len(sel)<=0:
       print( "ERROR: Select at least one object." )

   bbox = cmds.exactWorldBoundingBox(sel)
   bottom = [(bbox[0] + bbox[3])/2, (bbox[1] + bbox[4])/2, bbox[2]]
   cmds.xform(sel, piv=bottom, ws=True)
   cmds.move(0,0,0, sel, rotatePivotRelative = True)

selection = cmds.ls(sl=True)
for sel in selection:
    moveToCenter(sel)
    cmds.makeIdentity(sel, apply=True) 