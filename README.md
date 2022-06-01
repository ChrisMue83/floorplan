floorplan V 1.0


floorplan prints the outline of a defined area from a input textfile.

usage:
"floorplan textinputfile.txt > output.svg"

The input text file contains lengths and angles that have been previously measured. Thus, the floor plan 
is output graphically in an SVG file format, as well as the area is calculated for defined areas. 


The input file consists of three different logical parts:
a)comments and empty lines
b)lenghts and angles
c)control commands


a) 
Comments
begin always with a hashtag "#" 
All comments but also empty lines are ignored by the interpreter.

b) 
lengths and angles
Each length and angle gets a new line and is written as a pair. (eg. 2 90)

c) control elements:
begin with "->"
Restart: deletes all internal saved coordinates and begin with the coordinates (0,0)
Color: sets the color of all following lines (e.g. Color:green; ). The option "Color:blank;" doesnt show lines at all.
		all color names correspond with the svg format.
Width: sets the width (e.g. width:2; )
SavePoint: gives the last calculated point a name (e.g. SavePoint:KitchenCorner1; )
GoToPoint: jumps to the point which was saved by SavePoint (e.g. GoToPoint:KitchenCorner1; )
JoinPoint: joins the last calculated point with the Point given by SavePoint. (e.g JoinPoint:KitchenCorner1;)
GoToXY: goes to the X and Y Coordinates given (e.g. GoToXY:0 30)
SetAngle: Absolute starting angle is defined
ShowLength: shows all lengths in the diagram
HideLength: hides all lengths in the diagram
NoHeader: no svg header is generated
NoEnding: no svg ending is generated
BeginArea: begin of the polygon where the area shall be calculated
EndArea: end of the polygon where the area is calculated
TextSizeLine: Set Text size of the lines (e.g. TextSizeLine:7;)
TextSizeArea: Set Text size of the Area (e.g. TextSizeArea:20;)
Title: Name of the floorplan (e.g. Title: MyAppartment; )
RoomName: Room name (e.g. RoomName: "kitchen") . Roomname can only be used together with BeginArea and EndArea
CompleteArea: Calculates the sum of all areas which are calculated with BeginArea and EndArea
