include <roundedcube.scad>

$fn = 100;
MARGIN = 0.01;
WALL = 1.0;
ROUND = 2;

WIDTH = 60;
DEPTH = 25; // IKEA GLASS
// DEPTH = 23; // ALUMINUM GLASS

GLASS_TOP_DIA = 44;    // IKEA GLASS
GLASS_BOTTOM_DIA = 39; // IKEA GLASS
// GLASS_TOP_DIA = 32;    // ALUMINUM GLASS
// GLASS_BOTTOM_DIA = 25; // ALUMINUM GLASS
GLASS_HEIGHT = 20;
GLASS_WALL = 2.0;

PIN_DIA = 7;
PIN_HEIGT = 8.5;
PIN_DISTANCE = 14.6 + PIN_DIA;

CUT = 23;

difference()
{
    roundedcube([ WIDTH, DEPTH, PIN_HEIGT + WALL ], false, ROUND, "z");
    translate([ WIDTH / 2 - PIN_DISTANCE / 2, DEPTH / 2, -MARGIN ]) cylinder(d = PIN_DIA, h = PIN_HEIGT);
    translate([ WIDTH / 2 + PIN_DISTANCE / 2, DEPTH / 2, -MARGIN ]) cylinder(d = PIN_DIA, h = PIN_HEIGT);
}

translate([ WIDTH / 2, DEPTH / 2, PIN_HEIGT + WALL ]) intersection()
{
    difference()
    {
        cylinder(d1 = GLASS_BOTTOM_DIA + 2 * GLASS_WALL, d2 = GLASS_TOP_DIA + 2 * GLASS_WALL, h = GLASS_HEIGHT);
        cylinder(d1 = GLASS_BOTTOM_DIA, d2 = GLASS_TOP_DIA, h = GLASS_HEIGHT + MARGIN);
        // translate([ -CUT / 2, -DEPTH / 2 - MARGIN, 0 ]) cube([ CUT, DEPTH + 2 * MARGIN, GLASS_HEIGHT ]);
    }
    translate([ -WIDTH / 2, -DEPTH / 2, 0 ]) cube([ WIDTH, DEPTH, GLASS_HEIGHT ]);
    // translate([ -WIDTH / 2, -CUT / 2, 0 ]) cube([ WIDTH, CUT, GLASS_HEIGHT ]);
}