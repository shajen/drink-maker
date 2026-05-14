include <pipe.scad>
include <roundedcube.scad>
include <straigt_track.scad>

$fn = 100;
MARGIN = 0.01;
ROUND = 1.5;
WALL = 2.0;
LID_STEP = 1.0;
LID_MARGIN = 0.4;

WIDTH = 90 + 4 * WALL;
DEPTH = 120 + 4 * WALL;
HEIGHT = 52;

PIPE_IN_DIA = 6.6;
PIPE_OUT_DIA = 9.8;
PIPE_MOUNT_DIA = 10.0;
PIPE_CONNECTOR_LENGTH = 10;
PIPE_DEPTH_OFFSET = 73;
PIPE_BEND_R = 20;
PIPE_A = 110;
PIPE_B = 86;
PIPE_C = 50;

DS_DEPTH = 15;
DS_DEPTH_OFFSET = PIPE_DEPTH_OFFSET + DS_DEPTH + 5 + PIPE_MOUNT_DIA;
DS_HEIGHT = 8;
DS_HEIGHT_OFFSET = 26;

LCD_DEPTH = 44;
LCD_DEPTH_OFFSET = 22 + 14;
LCD_HEIGHT = 34;
LCD_HEIGHT_OFFSET = 10;

ESP_CABLE_DEPTH = 12.5;
ESP_CABLE_HEIGHT = 4.5;
ESP_CABLE_DEPTH_OFFSET = 82;
ESP_CABLE_HEIGHT_OFFSET = 36;

BASE_HEIGHT = 12;
BASE_WALL = 2;
BASE_MARGIN = 0.4;

TRACK_DISTANCE = 40;
TRACK_MARGIN = 2;

SWITCH_R = 6.2;
SWITCH_DEPTH_OFFSET = 110;
SWITCH_HEIGHT_OFFSET = 36;

MOUNTPOINT_SIZE = 6;
MOUNTPOINT_R = 1.4;
MOUNTPOINT_HEIGHT = 12;
SUPPORT_HEIGHT = 12;

module window(depth, height, depth_offset, height_offset) {
  translate([WALL + WIDTH - MARGIN, WALL + depth_offset - depth / 2, WALL + height_offset])
    cube([WALL + 2 * MARGIN, depth, height]);
}

module window2(depth, height, depth_offset, height_offset) {
  translate([-MARGIN, WALL + depth_offset - depth / 2, WALL + height_offset])
    cube([WALL + 2 * MARGIN, depth, height]);
}

module circle2(radius, depth_offset, height_offset) {
  translate([-MARGIN, depth_offset, height_offset + radius])
    rotate([0, 90, 0])
      cylinder(h=WALL + 2 * MARGIN, r=radius);
}

module single_support() {
  x = MOUNTPOINT_SIZE;
  h = SUPPORT_HEIGHT;
  translate([-MOUNTPOINT_SIZE / 2, -MOUNTPOINT_SIZE / 2, 0])
    polyhedron(
      points=[
        [0, 0, 0],
        [x, 0, 0],
        [0, 0, h],
        [x, 0, h],
        [x, x, h],
        [0, x, h],
      ],
      faces=[
        [0, 1, 3, 2],
        [1, 4, 3],
        [0, 5, 4, 1],
        [0, 2, 5],
        [2, 3, 4, 5],
      ],
      convexity=10
    );
}

module support(degree) {
  translate([MOUNTPOINT_SIZE / 2, MOUNTPOINT_SIZE / 2, 0]) rotate([0, 0, degree]) single_support();
  translate([MOUNTPOINT_SIZE / 2, MOUNTPOINT_SIZE / 2, 0]) rotate([0, 0, degree + 90]) single_support();
}

module mountpoint(x, y, degree) {
  translate([x + WALL, y + WALL, HEIGHT + WALL - SUPPORT_HEIGHT - MOUNTPOINT_HEIGHT]) {
    support(degree);
    translate([0, 0, SUPPORT_HEIGHT]) {
      difference() {
        cube([MOUNTPOINT_SIZE, MOUNTPOINT_SIZE, MOUNTPOINT_HEIGHT]);
        translate([MOUNTPOINT_SIZE / 2, MOUNTPOINT_SIZE / 2, 0])
          cylinder(h=MOUNTPOINT_HEIGHT + MARGIN, r=MOUNTPOINT_R);
      }
    }
  }
}

module shape(height, margin) {
  difference() {
    cube([WIDTH + 2 * margin, DEPTH + 2 * margin, height]);
    translate([WIDTH - WALL - PIPE_MOUNT_DIA + 2 * margin, PIPE_DEPTH_OFFSET + 2 * margin, -MARGIN]) cube(
        [
          PIPE_MOUNT_DIA + 2 * WALL + MARGIN + margin,
          PIPE_MOUNT_DIA + 4 * WALL - 2 * margin,
          height + 2 * MARGIN,
        ]
      );
  }
}

module main() {
  difference() {
    roundedcube([2 * WALL + WIDTH, 2 * WALL + DEPTH, 2 * WALL + HEIGHT], false, ROUND, "z");
    translate([WALL - LID_STEP, WALL - LID_STEP, WALL + HEIGHT]) shape(WALL + MARGIN, LID_STEP);
    translate([WALL, WALL, WALL]) shape(HEIGHT + WALL + MARGIN, 0);

    window(LCD_DEPTH, LCD_HEIGHT, LCD_DEPTH_OFFSET, LCD_HEIGHT_OFFSET);
    window(DS_DEPTH, DS_HEIGHT, DS_DEPTH_OFFSET, DS_HEIGHT_OFFSET);

    window2(
      ESP_CABLE_DEPTH, ESP_CABLE_HEIGHT, ESP_CABLE_DEPTH_OFFSET,
      ESP_CABLE_HEIGHT_OFFSET
    );
    circle2(SWITCH_R, SWITCH_DEPTH_OFFSET, SWITCH_HEIGHT_OFFSET);

    translate(
      [WALL + WIDTH - PIPE_MOUNT_DIA / 2, PIPE_DEPTH_OFFSET + PIPE_MOUNT_DIA / 2 + 3 * WALL, WALL + HEIGHT - PIPE_CONNECTOR_LENGTH]
    )
      pipe(0, PIPE_MOUNT_DIA, PIPE_CONNECTOR_LENGTH + WALL + MARGIN);

    translate([WIDTH - PIPE_MOUNT_DIA - MARGIN, PIPE_DEPTH_OFFSET + 3 * WALL, WALL + MARGIN])
      cube([PIPE_MOUNT_DIA + WALL, PIPE_MOUNT_DIA, HEIGHT - PIPE_CONNECTOR_LENGTH - WALL]);
  }
  mountpoint(0, 0, 270);
  mountpoint(0, DEPTH - 3 * WALL, 180);
  mountpoint(WIDTH - 3 * WALL, DEPTH - 3 * WALL, 90);
  mountpoint(WIDTH - 3 * WALL, 0, 0);
}

module lid() {
  shape(WALL, LID_STEP - LID_MARGIN);
}

module base() {
  difference() {
    roundedcube(
      [WIDTH + 2 * (BASE_WALL + WALL), DEPTH + 2 * (BASE_WALL + WALL), BASE_HEIGHT + BASE_WALL], false,
      ROUND, "z"
    );
    translate([BASE_WALL - BASE_MARGIN, BASE_WALL - BASE_MARGIN, BASE_HEIGHT])
      cube([WIDTH + 2 * (BASE_MARGIN + WALL), DEPTH + 2 * (BASE_MARGIN + WALL), BASE_WALL + MARGIN]);
    translate([2 * BASE_WALL, 2 * BASE_WALL, -MARGIN])
      cube([WIDTH + 2 * (WALL - BASE_WALL), DEPTH + 2 * (WALL - BASE_WALL), BASE_HEIGHT + 2 * MARGIN]);
  }

  X = 2 * (BASE_WALL + WALL);
  TRACK_LENGTH = DEPTH + 2 * (BASE_WALL + WALL);

  translate([WIDTH + X - TRACK_MARGIN, 0, 0]) cube([TRACK_DISTANCE + 2 * TRACK_MARGIN, DEPTH + X, WALL]);
  translate([WIDTH + X + TRACK_DISTANCE, TRACK_LENGTH, 0]) rotate([0, 0, 270])
      straigt_track(TRACK_LENGTH, false, left_connector="male", right_connector="female");
}

module pipe_u() {
  X = PIPE_BEND_R + PIPE_OUT_DIA / 2;
  Y = PIPE_BEND_R + PIPE_OUT_DIA / 2;

  A = PIPE_A - Y;
  B = PIPE_B - 2 * Y;
  C = PIPE_C - Y;

  translate([PIPE_OUT_DIA / 2, 0, X]) pipe(PIPE_IN_DIA, PIPE_OUT_DIA, A);
  translate([X, 0, X]) rotate([270, 0, 180]) pipe_90(PIPE_IN_DIA, PIPE_OUT_DIA, 0, PIPE_BEND_R);
  translate([X, 0, PIPE_OUT_DIA / 2]) rotate([0, 90, 0]) pipe(PIPE_IN_DIA, PIPE_OUT_DIA, B);
  translate([X + B, 0, X]) rotate([270, 0, 0]) pipe_90(PIPE_IN_DIA, PIPE_OUT_DIA, 0, PIPE_BEND_R);
  translate([B + 2 * X - PIPE_OUT_DIA / 2, 0, X]) pipe(PIPE_IN_DIA, PIPE_OUT_DIA, C);
}

translate([0 * (WIDTH + 10), 0, 0]) main();
translate([1 * (WIDTH + 10), 0, 0]) lid();
translate([2 * (WIDTH + 10), 0, 0]) base();
translate([3 * (WIDTH + 10) + TRACK_DISTANCE + 40 + 4 * WALL, 0, 0]) pipe_u();
