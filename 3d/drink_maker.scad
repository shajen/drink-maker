include <pipe.scad>
include <roundedcube.scad>
include <straigt_track.scad>

$fn = 100;
MARGIN = 0.01;
ROUND = 1.5;
WALL = 2.0;
LID_STEP = 1.0;
LID_MARGIN = 0.4;

WIDTH = 60;
DEPTH = 124;
HEIGHT = 52;

WS_IN_DIA = 0;
WS_OUT_DIA = 52;
WS_WIDTH = 3.5;
WS_DEPTH_OFFSET = WS_OUT_DIA / 2 + WALL;

PIPE_IN_DIA = 6.6;
PIPE_OUT_DIA = 9.8;
PIPE_MOUNT_DIA = 10.0;
PIPE_CONNECTOR_LENGTH = 10;
PIPE_DEPTH_OFFSET = WS_DEPTH_OFFSET + PIPE_MOUNT_DIA / 2 + WS_OUT_DIA / 2 + 2 * WALL;
PIPE_BEND_R = 20;
PIPE_A = 110;
PIPE_B = 86;
PIPE_C = 50;

DS_DEPTH = 15;
DS_DEPTH_OFFSET = PIPE_DEPTH_OFFSET + DS_DEPTH + 5;
DS_HEIGHT = 8;
DS_HEIGHT_OFFSET = 26;

LCD_DEPTH = 25.5;
LCD_DEPTH_OFFSET = DS_DEPTH_OFFSET + DS_DEPTH / 2 + LCD_DEPTH / 2 + 5;
LCD_HEIGHT = 14;
LCD_HEIGHT_OFFSET = 26;

ESP_WIDTH = 22;
ESP_WIDTH_OFFSET = 3;
ESP_DEPTH = 87;
ESP_DEPTH_OFFSET = 3;
ESP_CABLE_DEPTH = 14;
ESP_CABLE_HEIGHT = 8;
ESP_CABLE_DEPTH_OFFSET = 22;
ESP_CABLE_HEIGHT_OFFSET = 0;
ESP_HEIGHT_MOUNTPOINT = 24;

BASE_HEIGHT = 12;
BASE_WALL = 2;
BASE_MARGIN = 0.4;

TRACK_DISTANCE = 40;
TRACK_MARGIN = 2;

module window(depth, height, depth_offset, height_offset)
{
    translate([ WALL + WIDTH - MARGIN, WALL + depth_offset - depth / 2, WALL + height_offset ])
        cube([ WALL + 2 * MARGIN, depth, height ]);
}

module shape(height, margin)
{
    difference()
    {
        cube([ WIDTH + 2 * margin, DEPTH + 2 * margin, height ]);
        // translate(
        //     [ WIDTH - WALL - WS_WIDTH + 2 * margin, WS_DEPTH_OFFSET - WS_OUT_DIA / 2 - WALL + 2 * margin, -MARGIN ])
        //     roundedcube(
        //         [ WS_WIDTH + 2 * WALL + MARGIN + margin, WS_OUT_DIA + 2 * WALL - 2 * margin, height + 2 * MARGIN ],
        //         false, ROUND, "z");
        // translate([
        //     WIDTH - WALL - PIPE_MOUNT_DIA + 2 * margin, PIPE_DEPTH_OFFSET - PIPE_MOUNT_DIA / 2 - WALL + 2
        //     * margin, -MARGIN
        // ])
        //     roundedcube(
        //         [
        //             PIPE_MOUNT_DIA + 2 * WALL + MARGIN + margin, PIPE_MOUNT_DIA + 2 * WALL - 2 * margin,
        //             height + 2 *
        //             MARGIN
        //         ],
        //         false, ROUND, "z");
        translate([ WIDTH - WALL - PIPE_MOUNT_DIA + 2 * margin, 0, -MARGIN ]) cube([
            PIPE_MOUNT_DIA + 2 * WALL + MARGIN + margin, WS_OUT_DIA + PIPE_MOUNT_DIA + 4 * WALL, height + 2 * MARGIN
        ]);
    }
}

module main()
{
    difference()
    {
        roundedcube([ 2 * WALL + WIDTH, 2 * WALL + DEPTH, 2 * WALL + HEIGHT ], false, ROUND, "z");
        translate([ WALL - LID_STEP, WALL - LID_STEP, WALL + HEIGHT ]) shape(WALL + MARGIN, LID_STEP);
        translate([ WALL, WALL, WALL ]) shape(HEIGHT + WALL + MARGIN, 0);

        window(LCD_DEPTH, LCD_HEIGHT, LCD_DEPTH_OFFSET, LCD_HEIGHT_OFFSET);
        window(DS_DEPTH, DS_HEIGHT, DS_DEPTH_OFFSET, DS_HEIGHT_OFFSET);

        translate(
            [ WALL + WIDTH - PIPE_MOUNT_DIA / 2, PIPE_DEPTH_OFFSET + WALL, WALL + HEIGHT - PIPE_CONNECTOR_LENGTH ])
            pipe(0, PIPE_MOUNT_DIA, PIPE_CONNECTOR_LENGTH + WALL + MARGIN);

        translate([ WIDTH - PIPE_MOUNT_DIA - MARGIN, PIPE_DEPTH_OFFSET - PIPE_MOUNT_DIA / 2 + WALL, WALL + MARGIN ])
            cube([ PIPE_MOUNT_DIA + WALL, PIPE_MOUNT_DIA, HEIGHT - PIPE_CONNECTOR_LENGTH - WALL ]);

        translate([
            -MARGIN, WALL + ESP_DEPTH_OFFSET + ESP_CABLE_DEPTH_OFFSET, WALL + ESP_HEIGHT_MOUNTPOINT +
            ESP_CABLE_HEIGHT_OFFSET
        ]) cube([ WALL + 2 * MARGIN, ESP_CABLE_DEPTH, ESP_CABLE_HEIGHT ]);

        translate([ 2 * WALL + WIDTH - MARGIN - WS_WIDTH, WALL + WS_DEPTH_OFFSET, WALL + WS_OUT_DIA / 2 ])
            rotate([ 0, 90, 0 ]) pipe(WS_IN_DIA, WS_OUT_DIA, WS_WIDTH + 2 * MARGIN);
    };

    translate([ WALL + ESP_WIDTH_OFFSET, WALL + ESP_DEPTH_OFFSET, WALL ])
        pcb_mount(ESP_WIDTH, ESP_DEPTH, 2, 6, ESP_HEIGHT_MOUNTPOINT);
}

module base()
{
    difference()
    {
        roundedcube([ WIDTH + 2 * (BASE_WALL + WALL), DEPTH + 2 * (BASE_WALL + WALL), BASE_HEIGHT + BASE_WALL ], false,
                    ROUND, "z");
        translate([ BASE_WALL - BASE_MARGIN, BASE_WALL - BASE_MARGIN, BASE_HEIGHT ])
            cube([ WIDTH + 2 * (BASE_MARGIN + WALL), DEPTH + 2 * (BASE_MARGIN + WALL), BASE_WALL + MARGIN ]);
        translate([ 2 * BASE_WALL, 2 * BASE_WALL, -MARGIN ])
            cube([ WIDTH + 2 * (WALL - BASE_WALL), DEPTH + 2 * (WALL - BASE_WALL), BASE_HEIGHT + 2 * MARGIN ]);
    }

    X = 2 * (BASE_WALL + WALL);
    TRACK_LENGTH = DEPTH + 2 * (BASE_WALL + WALL);

    translate([ WIDTH + X - TRACK_MARGIN, 0, 0 ]) cube([ TRACK_DISTANCE + 2 * TRACK_MARGIN, DEPTH + X, WALL ]);
    translate([ WIDTH + X + TRACK_DISTANCE, TRACK_LENGTH, 0 ]) rotate([ 0, 0, 270 ])
        straigt_track(TRACK_LENGTH, false, left_connector = "male", right_connector = "female");
}

module pipe_u()
{
    X = PIPE_BEND_R + PIPE_OUT_DIA / 2;
    Y = PIPE_BEND_R + PIPE_OUT_DIA / 2;

    A = PIPE_A - Y;
    B = PIPE_B - 2 * Y;
    C = PIPE_C - Y;

    translate([ PIPE_OUT_DIA / 2, 0, X ]) pipe(PIPE_IN_DIA, PIPE_OUT_DIA, A);
    translate([ X, 0, X ]) rotate([ 270, 0, 180 ]) pipe_90(PIPE_IN_DIA, PIPE_OUT_DIA, 0, PIPE_BEND_R);
    translate([ X, 0, PIPE_OUT_DIA / 2 ]) rotate([ 0, 90, 0 ]) pipe(PIPE_IN_DIA, PIPE_OUT_DIA, B);
    translate([ X + B, 0, X ]) rotate([ 270, 0, 0 ]) pipe_90(PIPE_IN_DIA, PIPE_OUT_DIA, 0, PIPE_BEND_R);
    translate([ B + 2 * X - PIPE_OUT_DIA / 2, 0, X ]) pipe(PIPE_IN_DIA, PIPE_OUT_DIA, C);
}

translate([ 0 * (WIDTH + 10), 0, 0 ]) main();
translate([ 1 * (WIDTH + 10), 0, 0 ]) shape(WALL, LID_STEP - LID_MARGIN);
translate([ 2 * (WIDTH + 10), 0, 0 ]) base();
translate([ 3 * (WIDTH + 10) + TRACK_DISTANCE + 40 + 4 * WALL, 0, 0 ]) pipe_u();