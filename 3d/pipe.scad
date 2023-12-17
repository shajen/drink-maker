module pipe(inside_dia, outside_dia, height)
{
    linear_extrude(height = height, center = false) difference()
    {
        circle(r = outside_dia / 2, $fn = 100);
        circle(r = inside_dia / 2, $fn = 100);
    }
}

module pipe_90(in_dia, out_dia, length, bend_radius)
{
    X = 0; // bend_radius * sqrt(2);
    translate([ -X / 2, -X / 2, 0 ])
    {
        rotate_extrude(angle = 90, convexity = 10) translate([ bend_radius, 0, 0 ]) difference()
        {
            circle(r = out_dia / 2);
            circle(r = in_dia / 2);
        }
        translate([ -length, bend_radius, 0 ]) rotate([ 0, 90, 0 ]) pipe(in_dia, out_dia, length);
        translate([ bend_radius, 0, 0 ]) rotate([ 90, 0, 0 ]) pipe(in_dia, out_dia, length);
    }
}
