/*--- Rock ---*/

protected func Construction()
{
	var graphic = Random(10);
	if(graphic)
		SetGraphics(Format("%d",graphic));
}

protected func Hit()
{
	Sound("RockHit*");
	return 1;
}

local Collectible = 1;
local Name = "$Name$";
local Description = "$Description$";