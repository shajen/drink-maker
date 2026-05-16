MARGIN = 0.01;

module custom_cylinder(outside_radius, height, inside_radius = 0) {
  difference() {
    cylinder(h=height, r=outside_radius);
    if (0 < inside_radius) {
      translate([0, 0, -MARGIN])
        cylinder(h=height + 2 * MARGIN, r=inside_radius);
    }
  }
}
