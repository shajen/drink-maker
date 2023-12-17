/**
 * Some cool Tracks build with an modifed version from dotscad
 * [dotscad](https://github.com/dotscad/dotscad) by Chris Petersen
 *
 * @copyright  Smuk3D, 2014 http://www.thingiverse.com/Smuk3D/designs
 * @license    http://creativecommons.org/licenses/LGPL/2.1/
 * @license    http://creativecommons.org/licenses/by-sa/3.0/
 *
 * @see        http://www.thingiverse.com/thing:216944
 * @source     https://github.com/dotscad/trains/blob/master/track-wooden/curved-track.scad
 */

// the trick with negative option
difference()
{
    mytrack(false);
    mytrack(true);
}

// perhaps BRIO sizes .. i dont own BRIO tracks
l1_1 = 216;

l2_3 = 144;
l1_3 = 72;

l1_2 = 108;

// billig schinene
l_klein = 60;
l_mittel = 150;
l_lang = 200;

// gerade();
// kurve();
// auffahrt();

// with negative feature
module mytrack(neg = false)
{
    // kreuzungKurve(neg);
    // kreuzungKurve2(neg);
    // kreuzung(true,true,neg); //kreuzungGeradeUndKurve
    // kreuzung(false,true,neg); //kreuzungGerade
    // kreuzung(true,false,neg); //kreuzungKurve3
    // kreuzungSechsfach(neg,angle=60);
    // kreuzungSechsfach(neg,angle=120); //kreuzungSechsfach2
    // kreuzungX(neg);
    // weiche(true,true,true,neg); //weicheLinksMitteRechts
    // weiche(true,false,true,neg); //weicheLinksRechts
    // weiche(true,true,false,neg,trackOnBottom=true); //weicheLinksMitte
    // weicheT(neg);
    // weicheT(neg, l=l_mittel); //weicheTklein
    // weicheKurve(neg);
    // weicheFaecher(neg);
}

// Is only a Hack an not very good implemented
module auffahrt(neg = false, $fn = 100)
{
    right_connector = "male";
    o = .1;
    bevel_width = 1;
    bevel = o + bevel_width;

    well_width = wood_well_width();
    well_spacing = wood_well_spacing();
    well_padding = (wood_width() - well_spacing - (2 * well_width)) / 2;
    bevel_pad = bevel_width * sqrt(.5) * (o / 2);

    length = l_klein;

    difference()
    {
        union()
        {
            translate([ 0, 0, 0 ]) rotate([ 90, 0, 90 ]) linear_extrude(20, convexity = 10) wood_track_2d();
            translate([ 20, 0, 0 ]) rotate([ 90, 0, 90 ])
                linear_extrude(length - 20, convexity = 10, scale = [ 1, 0.05 ]) wood_track_2d();
            if (right_connector == "male")
            {
                translate([ 0, wood_width() / 2, 0 ]) rotate([ 0, 0, 180 ]) wood_plug();
            }
        }
        if (right_connector == "female")
        {
            translate([ 0, wood_width() / 2, 0 ]) rotate([ 0, 0, 0 ]) wood_cutout();
        }
        // Bevels on outer faces of the wheel wells
        for (i = [
                 well_padding + bevel_pad, well_padding + well_width - bevel_pad,
                 wood_width() - well_padding - well_width + bevel_pad, wood_width() - well_padding -
                 bevel_pad
             ])
        {
            for (j = [ -bevel_pad, length + bevel_pad ])
            {
                translate(v = [ j, i, wood_height() - ((wood_height() - wood_well_height() - o) / 2) ])
                {
                    rotate(a = [ 0, 0, 45 ])
                    {
                        cube(size = [ bevel, bevel, wood_height() - wood_well_height() + o ], center = true);
                    }
                }
            }
        }
    }
}

// some straigt tracks
module gerade()
{
    left_connector = "male";
    right_connector = "female";

    translate([ -100, 0, 0 ])
        straigt_track(l_klein, false, left_connector = left_connector, right_connector = left_connector);
    translate([ -100, 50, 0 ])
        straigt_track(l_klein, false, left_connector = right_connector, right_connector = right_connector);

    translate([ 0, 0, 0 ])
        straigt_track(l_klein, false, left_connector = left_connector, right_connector = right_connector);

    translate([ 100, 0, 0 ])
        straigt_track(50, false, left_connector = left_connector, right_connector = right_connector);

    translate([ 200, 0, 0 ])
        straigt_track(25, false, left_connector = left_connector, right_connector = right_connector);

    translate([ 0, 50, 0 ])
        straigt_track(l_mittel, false, left_connector = left_connector, right_connector = right_connector);

    translate([ 200, 50, 0 ])
        straigt_track(l_mittel / 2, false, left_connector = left_connector, right_connector = right_connector);

    translate([ 0, 100, 0 ])
        straigt_track(l_lang, false, left_connector = left_connector, right_connector = right_connector);

    translate([ 240, 100, 0 ])
        straigt_track(l_lang / 2, false, left_connector = left_connector, right_connector = right_connector);
}

// some curved tracks
module kurve()
{
    left_connector = "male";
    right_connector = "female";

    for (radius = [ 87.5, 180 ])
    {
        translate([ -100, -200, 0 ]) curved_track(radius, 45 / 2, neg = false, left_connector = left_connector,
                                                  right_connector = right_connector, trackOnBottom = true);
        translate([ 200, -200, 0 ]) curved_track(radius, 30, neg = false, left_connector = left_connector,
                                                 right_connector = right_connector, trackOnBottom = true);
        translate([ -100, 0, 0 ]) curved_track(radius, 45, neg = false, left_connector = left_connector,
                                               right_connector = right_connector, trackOnBottom = true);
        translate([ 200, 0, 0 ]) curved_track(radius, 90, neg = false, left_connector = left_connector,
                                              right_connector = right_connector, trackOnBottom = true);
    }
}

// FERTIG
module kreuzungKurve(neg = false)
{
    l = l_mittel;
    radius = 205;
    angle = 45;
    left_connector = "female";
    right_connector = "female";
    top_connector = "female";
    bottom_connector = "female";
    offset = wood_well_width() / 2;

    for (r = [ 0, 180 ])
    {
        rotate([ 0, 0, r ]) translate([ -radius - wood_width() / 2 - offset / 2, 0, 0 ]) rotate([ 0, 0, -angle / 2 ])
            curved_track(radius, angle, neg = neg, left_connector = top_connector, right_connector = right_connector);
    }
}

// FERTIG
module weicheFaecher(neg = false)
{
    l = l_mittel;

    top_connector = "female";
    bottom_connector = "female";

    translate([ 0, -wood_width() / 2, 0 ])
        straigt_track(l, neg, left_connector = top_connector, right_connector = bottom_connector);

    radius = 228;
    angle = 45 / 2;
    radius2 = 99;
    angle2 = 45;

    for (i = [ 0, 1 ])
    {
        mirror([ 0, i, 0 ])
        {
            rotate([ 0, 0, -90 ]) translate([ -radius - wood_width() / 2, 0, 0 ])
                curved_track(radius, angle, neg, left_connector = "none", right_connector = bottom_connector);
            translate([ 0, radius + wood_width() / 2, 0 ]) rotate([ 0, 0, angle - 0.01 ])
                translate([ 0, -radius - wood_width(), 0 ])
                    straigt_track(l - 98.65, neg, left_connector = "none", right_connector = top_connector);
            rotate([ 0, 0, -90 ]) translate([ -radius2 - wood_width() / 2, 0, 0 ])
                curved_track(radius2, angle2, neg, left_connector = "none", right_connector = bottom_connector);
            translate([ 0, radius2 + wood_width() / 2, 0 ]) rotate([ 0, 0, angle2 - 0.01 ])
                translate([ 0, -radius2 - wood_width(), 0 ])
                    straigt_track(l - 98.6, neg, left_connector = "none", right_connector = top_connector);
        }
    }

    // mini woodwellremove
    if (neg)
    {
        translate([ 50, -wood_well_spacing() / 2 - wood_well_width() / 2, wood_well_height() ])
            cube([ 20, wood_well_spacing() + wood_well_width(), wood_well_height() ]);
    }
}

// FERTIG
module kreuzungX(neg = false, angle = 45)
{
    l = l_mittel;

    left_connector = "female";
    right_connector = "female";

    for (r = [ 0, angle ])
    {
        rotate([ 0, 0, r ]) translate([ -l / 2, -wood_width() / 2, 0 ])
            straigt_track(l, neg, left_connector = left_connector, right_connector = right_connector);
    }
}

// FERTIG
module kreuzungSechsfach(neg = false, angle = 60)
{
    l = l_mittel;

    connector = "female";

    for (r = [ 0, angle / 2, angle ])
    {
        rotate([ 0, 0, r ]) translate([ -l / 2, -wood_width() / 2, 0 ])
            straigt_track(l, neg, left_connector = connector, right_connector = connector);
    }
}

// FERTIG
module kreuzung(kurven = true, gerade = true, neg = false)
{

    l = kurven == true && gerade == true    ? l_lang
        : kurven == false && gerade == true ? l_mittel
        : kurven == true && gerade == false ? l_lang
                                            : l_mittel;

    radius = (l - wood_width()) / 2;
    angle = 90;
    connector = "female";

    if (kurven == true)
    {
        for (r = [ 0, 90, 180, 270 ])
        {
            rotate([ 0, 0, r ]) translate([ -l / 2, -l / 2, 0 ])
                curved_track(radius, angle, neg, left_connector = connector, right_connector = connector);
        }
    }

    if (gerade == true)
    {
        for (r = [ 0, 90 ])
        {
            rotate([ 0, 0, r ]) translate([ -(l / 2), -wood_width() / 2, 0 ])
                straigt_track(l, neg, left_connector = connector, right_connector = connector);
        }
    }
}

// FERTIG
module weicheT(neg = false, l = l_lang)
{
    // l=l_lang;
    radius = (l - wood_width()) / 2;
    angle = 90;
    top_connector = "female";
    bottom_connector = "female";

    for (m = [ 0, 1 ])
    {
        mirror([ m, 0, 0 ]) translate([ -radius - wood_width() / 2, 0, 0 ])
            curved_track(radius, angle, neg, left_connector = top_connector, right_connector = bottom_connector);
    }

    translate([ -l / 2, radius, 0 ])
        straigt_track(l, neg, left_connector = top_connector, right_connector = top_connector);
}

// FERTIG
module weicheKurve(neg = false)
{
    top_connector = "female";
    bottom_connector = "female";
    translate([ -180 - wood_width() / 2, 0, 0 ]) curved_track(180, 45, neg, left_connector = top_connector,
                                                              right_connector = bottom_connector, trackOnBottom = true);
    translate([ -87.5 - wood_width() / 2, 0, 0 ]) curved_track(
        87.5, 90, neg, left_connector = top_connector, right_connector = bottom_connector, trackOnBottom = true);
}

// FERTIG
module kreuzungKurve2(neg = false)
{
    radius = 180;
    angle = 45;
    offset = 10;
    left_connector = "female";
    right_connector = "male";
    for (m = [ 0, 1 ])
    {
        mirror([ m, 0, 0 ]) translate([ -radius + offset / 2, 0, 0 ])
            curved_track(radius, angle, neg, left_connector = left_connector, right_connector = right_connector);
    }
}

// FERTIG
module weiche(links = true, mitte = true, rechts = true, neg = false, trackOnBottom = false)
{
    l = l_mittel;
    radius = 180;
    angle = 45;
    left_connector = "female";
    right_connector = "female";
    if (links == true)
    {
        mirror([ 0, 0, 0 ]) translate([ -radius - wood_width() / 2, 0, 0 ])
            curved_track(radius, angle, neg, left_connector = left_connector, right_connector = right_connector,
                         trackOnBottom = trackOnBottom);
    }
    if (rechts == true)
    {
        mirror([ 1, 0, 0 ]) translate([ -radius - wood_width() / 2, 0, 0 ])
            curved_track(radius, angle, neg, left_connector = left_connector, right_connector = right_connector,
                         trackOnBottom = trackOnBottom);
    }
    if (mitte == true)
    {
        rotate([ 0, 0, 90 ]) translate([ 0, -wood_width() / 2, 0 ]) straigt_track(
            l, neg, left_connector = left_connector, right_connector = right_connector, trackOnBottom = trackOnBottom);
    }
}

/* ******************************************************************************
 * Main module code below:
 * ****************************************************************************** */

/*
 * @param int radius Radius of inner edge of the trac arc.  Standard track curves are 36cm and 17.5cm diameter.
 * @param int angle  Angle of track to render.  Standard track angle is 45 degrees.
 */
module curved_track(radius, angle, neg = false, left_connector = "none", right_connector = "none",
                    trackOnBottom = false, $fn = 100)
{
    right_bevels = right_connector == "none" ? false : true;
    left_bevels = left_connector == "none" ? false : true;

    difference()
    {
        union()
        {
            wood_track_arc(radius = radius, angle = angle, neg = neg, left_bevels = left_bevels,
                           right_bevels = right_bevels, trackOnBottom = trackOnBottom);

            if (neg == false)
            {
                if (right_connector == "male")
                {
                    translate([ radius + wood_width() / 2, 0, 0 ]) rotate([ 0, 0, -90 ]) wood_plug();
                }
                if (left_connector == "male")
                {
                    rotate([ 0, 0, angle ]) translate([ radius + wood_width() / 2, 0, 0 ]) rotate([ 0, 0, 90 ])
                        wood_plug();
                }
            }
        }
        if (right_connector == "female")
        {
            translate([ radius + wood_width() / 2, 0, 0 ]) rotate([ 0, 0, 90 ]) wood_cutout();
        }
        if (left_connector == "female")
        {
            rotate([ 0, 0, angle ]) translate([ radius + wood_width() / 2, 0, 0 ]) rotate([ 0, 0, -90 ]) wood_cutout();
        }
    }
}

module straigt_track(length, neg = false, left_connector = "none", right_connector = "none", trackOnBottom = false,
                     $fn = 100)
{
    right_bevels = right_connector == "none" ? false : true;
    left_bevels = left_connector == "none" ? false : true;

    difference()
    {
        union()
        {
            wood_track(length = length, neg = neg, left_bevels = left_bevels, right_bevels = right_bevels,
                       trackOnBottom = trackOnBottom);

            if (neg == false)
            {
                if (right_connector == "male")
                {
                    translate([ length, wood_width() / 2, 0 ]) wood_plug();
                }
                if (left_connector == "male")
                {
                    translate([ 0, wood_width() / 2, 0 ]) rotate([ 0, 0, 180 ]) wood_plug();
                }
            }
        }
        if (right_connector == "female")
        {
            translate([ length, wood_width() / 2, 0 ]) rotate([ 0, 0, 180 ]) wood_cutout();
        }
        if (left_connector == "female")
        {
            translate([ 0, wood_width() / 2, 0 ]) wood_cutout();
        }
    }
}

// tracklib

/**
 * A parametric library of modules for creating parts compatible with toy trains
 * (currently focused primarily on Thomas- and Brio-compatible wooden trains, as well as
 * Thomas Trackmaster (motorized plastic) and Take-N-Play (die cast).
 *
 * Some functions in this library require other [dotscad](https://github.com/dotscad/)
 * files.  I would recommend cloning that repository into the same parent directory
 * that contains this repository.
 *
 * This OpenSCAD library is part of the [dotscad](https://github.com/dotscad/dotscad)
 * project, an open collection of modules and Things for 3d printing.  Please check there
 * for the latest versions of this and other related files.
 *
 * @copyright  Chris Petersen, 2014
 * @copyright  Smuk3D, 2014 http://www.thingiverse.com/Smuk3D/designs
 * @license    http://creativecommons.org/licenses/LGPL/2.1/
 * @license    http://creativecommons.org/licenses/by-sa/3.0/
 *
 * @see        http://www.thingiverse.com/thing:216915
 * @source     https://github.com/dotscad/trains/blob/master/tracklib.scad
 */

// A global overlap variable (to prevent printing glitches)
o = .1;

// Constants for wooden track parts:
function wood_width() = 40;
function wood_height() = 12;
function wood_well_height() = 9;
function wood_well_width() = 5.7;
function wood_well_spacing() = 19.25;
function wood_plug_radius() = 6;
function wood_plug_neck_length() = 12;

// Constants for trackmaster parts
function trackmaster_width() = 40;
function trackmaster_height() = 12;
function trackmaster_well_height() = 8.4;
function trackmaster_plug_radius() = 3.8;
function trackmaster_plug_neck_length() = 4.75;

// @todo need to figure out what to call these variables...
// Bevel size
bevel_width = 1;
bevel = o + bevel_width;

/* ******************************************************************************
 * Include some other libraries
 * ****************************************************************************** */

// Not sure where the main dotscad files might be, so try to load from a few locations.

module pie(radius, angle, height, spin = 0)
{
    // Negative angles shift direction of rotation
    clockwise = (angle < 0) ? true : false;
    // Support angles < 0 and > 360
    normalized_angle = abs((angle % 360 != 0) ? angle % 360 : angle % 360 + 360);
    // Select rotation direction
    rotation = clockwise ? [ 0, 180 - normalized_angle ] : [ 180, normalized_angle ];
    // Render
    if (angle != 0)
    {
        rotate([ 0, 0, spin ]) linear_extrude(height = height) difference()
        {
            circle(radius);
            if (normalized_angle < 180)
            {
                union() for (a = rotation) rotate(a) translate([ -radius, 0, 0 ]) square(radius * 2);
            }
            else if (normalized_angle != 360)
            {
                intersection_for(a = rotation) rotate(a) translate([ -radius, 0, 0 ]) square(radius * 2);
            }
        }
    }
}

/*
 * When this file is opened directly in OpenSCAD, the following code will render an
 * example of the functions it provides.  This example will *not* render if this module
 * is imported into your own project via the `use` statement.
 */
// pie_example();
module pie_example($fn = 50)
{
    union()
    {
        pie(40, 45, 5);
        rotate([ 0, 0, -5 ]) difference()
        {
            sphere(r = 25);
            translate([ 0, 0, -30 ]) pie(50, -45, 60, 15);
        }
    }
}

/* ******************************************************************************
 * Examples
 * ****************************************************************************** */

/*
 * When this file is opened directly in OpenSCAD, the following code will render an
 * example of the functions it provides.  This example will *not* render if this module
 * is imported into your own project via the `use` statement.
 */
// tracklib_example();
module tracklib_example($fn = 25)
{
    // Wood pieces
    wood_track(10);
    translate([ 0, -60, 0 ]) wood_track(10, trackOnBottom = true);
    translate([ 15, 30, 0 ]) wood_plug();
    translate([ 15, 10, 0 ]) difference()
    {
        translate([ 0, -wood_plug_radius() - 2 ])
            cube([ wood_plug_neck_length() + wood_plug_radius() + 2, wood_plug_radius() * 2 + 4, wood_height() ]);
        wood_cutout();
    }
    translate([ -15, -10, 0 ]) rotate([ 0, 0, 90 ]) wood_track_arc(10, 25, $fn = 100);
    translate([ -15, -70, 0 ]) rotate([ 0, 0, 90 ]) wood_track_arc(10, 25, $fn = 100, trackOnBottom = true);

    translate([ -90, -10, 0 ]) difference()
    {
        mytrack(false);
        mytrack(true);
    }

    // Trackmaster pieces
    translate([ 40, 30, 0 ]) trackmaster_plug();
    translate([ 40, 10, 0 ]) difference()
    {
        translate([ 0, -trackmaster_plug_radius() - 2 ]) cube([
            trackmaster_plug_neck_length() + trackmaster_plug_radius() + 2, trackmaster_plug_radius() * 2 + 4,
            trackmaster_height()
        ]);
        trackmaster_cutout();
    }
}

/* ******************************************************************************
 * Modules useful to all varieties of train/track parts
 * ****************************************************************************** */

/**
 * Cutout (female) for track connector, centered on its Y axis.  Parameters to adjust for
 * wood or Trackmaster.
 * @param float radius      Radius of the cutout (recommended .3-.8 larger than plug)
 * @param float neck_length Length of the post's neck (edge of track to center of round cutout)
 */
module plug_cutout(radius, neck_length, track_height)
{
    bevel_pad = sqrt(.5) * (o / 2);
    bevel_height = sqrt(.5) * (bevel_width + o);
    bevel_radius = bevel_height - bevel_pad;
    height_pad = sqrt(.5) * (bevel_width / 2);
    union()
    {
        translate(v = [ -o, -3.75, -o ])
        {
            cube(size = [ o + neck_length, 7.5, track_height + o + o ]);
        }
        translate(v = [ neck_length, 0, track_height / 2 ])
        {
            cylinder(h = track_height + o + o, r = radius, center = true);
        }
        // bevelled edges
        translate(v = [ neck_length, 0, track_height + o - height_pad ])
        {
            cylinder(h = bevel_height + o, r1 = radius - bevel_pad, r2 = radius + bevel_radius, center = true);
        }
        translate(v = [ neck_length, 0, height_pad - o ])
        {
            cylinder(h = bevel_height + o, r1 = radius + bevel_radius, r2 = radius - bevel_pad, center = true);
        }
        for (i = [ 3.75 - bevel_pad, -3.75 + bevel_pad ])
        {
            for (j = [ track_height + bevel_pad, -bevel_pad ])
            {
                translate(v = [ (neck_length - o) / 2, i, j ])
                {
                    rotate(a = [ 45, 0, 0 ])
                    {
                        cube(size = [ o + neck_length, bevel, bevel ], center = true);
                    }
                }
            }
        }
        // @todo if track_height < wood_height() then extend cylinder and edges upward to wood_height() so plug is
        // properly subtracted from wood track
    }
}

/* ******************************************************************************
 * Modules dealing with wooden track/parts
 * ****************************************************************************** */

module wood_track_2d_neg(trackOnTop = true, trackOnBottom = false)
{
    well_width = wood_well_width();
    well_spacing = wood_well_spacing();
    well_padding = (wood_width() - well_spacing - (2 * well_width)) / 2;
    bevel_pad = bevel_width * sqrt(.5) * (o / 2);

    // Hole wells
    for (i = [ well_padding, wood_width() - well_padding - well_width ])
    {
        for (j = [ trackOnBottom == true ? 0 : 0 / 0, trackOnTop == true ? wood_well_height() : 0 / 0 ])
        {
            translate(v = [ i, j ])
            {
                square([ well_width, wood_height() - wood_well_height() + o ]);
            }
        }
    }

    // Bevels on wheel wells
    for (i = [
             well_padding + bevel_pad, well_padding + well_width - bevel_pad,
             wood_width() - well_padding - well_width + bevel_pad, wood_width() - well_padding -
             bevel_pad
         ])
    {
        for (j = [ trackOnBottom == true ? 0 : 0 / 0, trackOnTop == true ? wood_height() + bevel_pad : 0 / 0 ])
        {
            // top side
            translate(v = [ i, j ])
            {
                rotate(a = [ 0, 0, 45 ])
                {
                    square([ bevel, bevel ], center = true);
                }
            }
        }
    }
}

/**
 * 2d shape for basic wooden track.  To be used with linear_extrude() and rotate_extrude().
 */
module wood_track_2d(trackOnTop = true, trackOnBottom = false)
{
    well_width = wood_well_width();
    well_spacing = wood_well_spacing();
    well_padding = (wood_width() - well_spacing - (2 * well_width)) / 2;
    bevel_pad = bevel_width * sqrt(.5) * (o / 2);
    difference()
    {
        square(size = [ wood_width(), wood_height() ]);
        // Wheel wells
        for (i = [ well_padding, wood_width() - well_padding - well_width ])
        {
            for (j = [trackOnTop == true ? wood_well_height() : 0 / 0])
            {
                translate(v = [ i, j ])
                {
                    square([ well_width, wood_height() - wood_well_height() + o ]);
                }
            }
        }
        // Bevels on wheel wells
        for (i = [
                 well_padding + bevel_pad, well_padding + well_width - bevel_pad,
                 wood_width() - well_padding - well_width + bevel_pad, wood_width() - well_padding -
                 bevel_pad
             ])
        {
            for (j = [ trackOnBottom == true ? 0 : 0 / 0, trackOnTop == true ? wood_height() + bevel_pad : 0 / 0 ])
            {
                // top side
                translate(v = [ i, j ])
                {
                    rotate(a = [ 0, 0, 45 ])
                    {
                        square([ bevel, bevel ], center = true);
                    }
                }
            }
        }
        // Bevels on the track sides
        for (i = [
                 [ -bevel_pad, wood_height() + bevel_pad ], [ wood_width() + bevel_pad, -bevel_pad ],
                 [ -bevel_pad, -bevel_pad ], [ wood_width() + bevel_pad, wood_height() + bevel_pad ]
             ])
        {
            translate(v = i)
            {
                rotate(a = [ 0, 0, 45 ])
                {
                    square([ bevel, bevel ], center = true);
                }
            }
        }
    }
}

/**
 * Individual piece of wooden track.  Same gauge as Trackmaster but not the same shape.
 * @param int length Length of track to render.  Standard short wooden length is 53.5mm.
 */
module wood_track(length = 53.5, neg = false, left_bevels = true, right_bevels = true, trackOnTop = true,
                  trackOnBottom = false)
{
    well_width = wood_well_width();
    well_spacing = wood_well_spacing();
    well_padding = (wood_width() - well_spacing - (2 * well_width)) / 2;
    bevel_pad = bevel_width * sqrt(.5) * (o / 2);
    difference()
    {
        if (neg == true)
        {
            rotate([ 90, 0, 90 ]) linear_extrude(length, convexity = 10)
                wood_track_2d_neg(trackOnTop = trackOnTop, trackOnBottom = trackOnBottom);
        }
        else
        {
            rotate([ 90, 0, 90 ]) linear_extrude(length, convexity = 10)
                wood_track_2d(trackOnTop = trackOnTop, trackOnBottom = trackOnBottom);
        }

        // // Bevels on outer faces of the wheel wells
        // for (i = [
        //          well_padding + bevel_pad, well_padding + well_width - bevel_pad,
        //          wood_width() - well_padding - well_width + bevel_pad, wood_width() - well_padding -
        //          bevel_pad
        //      ])
        // {
        //     for (j = [ left_bevels == true ? -bevel_pad : 0 / 0, right_bevels == true ? length + bevel_pad : 0 / 0 ])
        //     {
        //         for (k = [
        //                  trackOnTop == true ? wood_height() - ((wood_height() - wood_well_height() - o) / 2) : 0 / 0,
        //                  trackOnBottom == true ? 0 + ((wood_height() - wood_well_height() + o) / 2) : 0 / 0
        //              ])
        //         {
        //             translate(v = [ j, i, k ])
        //             {
        //                 rotate(a = [ 0, 0, 45 ])
        //                 {
        //                     cube(size = [ bevel, bevel, wood_height() - wood_well_height() + o ], center = true);
        //                 }
        //             }
        //         }
        //     }
        // }
    }
}

/**
 * Individual piece of wooden track, curved along an arc.
 * @param int radius Radius of inner edge of the trac arc.  Standard track curves are 36cm and 17.5cm diameter.
 * @param int angle  Angle of track to render.  Standard track angle is 45 degrees.
 */
module wood_track_arc(radius = 245 / 2, angle = 45, neg = false, left_bevels = true, right_bevels = true,
                      trackOnTop = true, trackOnBottom = false)
{
    well_width = wood_well_width();
    well_spacing = wood_well_spacing();
    well_padding = (wood_width() - well_spacing - (2 * well_width)) / 2;
    bevel_pad = bevel_width * sqrt(.5) * (o / 2);

    difference()
    {
        intersection()
        {
            pie(radius + wood_width(), angle, wood_height());
            if (neg == true)
            {
                rotate_extrude(convexity = 10) translate([ radius, 0, 0 ])
                    wood_track_2d_neg(trackOnTop = trackOnTop, trackOnBottom = trackOnBottom);
            }
            else
            {
                rotate_extrude(convexity = 10) translate([ radius, 0, 0 ])
                    wood_track_2d(trackOnTop = trackOnTop, trackOnBottom = trackOnBottom);
            }
        }
        // Bevels on outer faces of the wheel wells
        for (a = [ right_bevels == true ? 0 : 0 / 0, left_bevels == true ? angle : 0 / 0 ])
        {
            rotate([ 0, 0, a ]) translate([ radius, 0, 0 ])
                rotate([ 0, 0, -90 ]) for (i = [
                                               well_padding + bevel_pad, well_padding + well_width - bevel_pad,
                                               wood_width() - well_padding - well_width + bevel_pad,
                                               wood_width() - well_padding -
                                               bevel_pad
                                           ])
            {
                for (k = [
                         trackOnTop == true ? wood_height() - ((wood_height() - wood_well_height() - o) / 2) : 0 / 0,
                         trackOnBottom == true ? 0 + ((wood_height() - wood_well_height() + o) / 2) : 0 / 0
                     ])
                {
                    translate(v = [ -bevel_pad, i, k ])
                    {
                        rotate(a = [ 0, 0, 45 ])
                        {
                            cube(size = [ bevel, bevel, wood_height() - wood_well_height() + o ], center = true);
                        }
                    }
                }
            }
        }
    }
}

/**
 * Plug (male) for wooden track, centered on its y axis.
 * @param bool solid Render as a solid plug, or set to false for the "spring" variant.
 */
module wood_plug(solid = true)
{
    neck_length = wood_plug_neck_length();
    // The width of the post depends on whether this is a "solid" or "spring" plug
    post_w = solid ? 6 : 3.5;
    // Render the part
    union()
    {
        translate(v = [ -o, -post_w / 2, 0 ]) hull()
            assign(adjusted_length = solid ? neck_length : neck_length + wood_plug_radius() - bevel_width - o)
        {
            translate([ 0, 0, 1 ]) cube(size = [ o + adjusted_length, post_w, wood_height() - 2 ]);
            translate([ 0, 1, 0 ]) cube(size = [ o + adjusted_length, post_w - 2, wood_height() ]);
        }
        difference()
        {
            translate(v = [ neck_length, 0, 0 ])
            {
                union()
                {
                    difference()
                    {
                        hull()
                        {
                            translate([ 0, 0, 1 ]) cylinder(h = wood_height() - 2, r = wood_plug_radius());
                            cylinder(h = wood_height(), r = wood_plug_radius() - bevel_width);
                        }
                        if (!solid)
                        {
                            translate(v = [ -6, -3.2, -o ]) cube(size = [ 6, 6.4, wood_height() + o + o ]);
                            translate(v = [ 0, 0, -o ]) cylinder(h = wood_height() + o + o, r = 3.8);
                            translate(v = [ -5, 0, wood_height() / 2 + o ]) rotate([ 0, 0, 45 ])
                                cube(size = [ 7, 7, wood_height() + o + o ], center = true);
                            translate(v = [ -5, 0, wood_height() / 2 + o ]) rotate([ 0, 0, 0 ])
                                cube(size = [ 2, 10, wood_height() + o + o ], center = true);
                        }
                    }
                }
            }
        }
    }
}

/**
 * Cutout (female) for wooden track, centered on its Y axis
 */
module wood_cutout()
{
    plug_cutout(wood_plug_radius() + .35, wood_plug_neck_length() - 1.2, wood_height());
}

/* ******************************************************************************
 * Modules dealing with Trackmaster compatible track/parts
 * ****************************************************************************** */

/**
 * Plug (male) for Trackmaster track, centered on its Y axis
 */
module trackmaster_plug()
{
    neck_length = trackmaster_plug_neck_length();
    difference()
    {
        union()
        {
            translate(v = [ -o, -2.5, 0 ])
            {
                hull()
                {
                    translate([ 0, 0, 1 ]) cube(size = [ o + neck_length, 5, trackmaster_well_height() - 2 ]);
                    translate([ 0, 1, 0 ]) cube(size = [ o + neck_length, 5 - 2, trackmaster_well_height() ]);
                }
            }
            translate(v = [ neck_length, 0, 0 ])
            {
                hull()
                {
                    cylinder(h = trackmaster_well_height(), r = trackmaster_plug_radius() - bevel_width);
                    translate([ 0, 0, 1 ]) cylinder(h = trackmaster_well_height() - 2, r = trackmaster_plug_radius());
                }
            }
        }
        translate(v = [ 2, -.6, -o ])
        {
            cube(size = [ 7 + o, 1.2, trackmaster_well_height() + o + o ]);
        }
        translate(v = [ 4.75, 0, -o ])
        {
            cylinder(h = trackmaster_well_height() + o + o, r = 1.75);
        }
    }
}

/**
 * Cutout (female) for Trackmaster track, centered on its Y axis
 */
module trackmaster_cutout()
{
    plug_cutout(trackmaster_plug_radius() + .7, trackmaster_plug_neck_length(), trackmaster_height());
}