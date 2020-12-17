$fn = 150;

cross_bar_thickness     = 1.13;
cross_height            = 4.52;
// The cross bar is off centered, so I'm making both bar dimensions equal to
// the longest bar to minimize fit issues.
//cross_width             = 3.76;
cross_width             = 4.52;
cross_z_height          = 5.6 + 0.2;
button_base_width       = 15;
button_base_length      = 15;
button_height           = 8;
button_offset           = 3;
button_height_scale     = 0.75;
button_top_neg_radius   = 40;
button_neg_z_pos        = button_height + 0.968 * button_top_neg_radius;

module button_crossbar() {
    color("purple")
    translate([-0.5 * cross_width, -0.5 * cross_bar_thickness, 0])
    cube([cross_width, cross_bar_thickness, cross_z_height]);

    translate([-0.5 * cross_bar_thickness, -0.5 * cross_height, 0])
    cube([cross_bar_thickness, cross_height, cross_z_height]);
}

module button_body() {
color("blue")
translate([0, 0, 0.1])
linear_extrude(height = button_height, center = false, convexity = 20, scale=button_height_scale)
translate([
    -0.5 * button_base_length,
    -0.5 * button_base_width,
    0.5 * button_height])
offset(r=button_offset)
square(size=[button_base_length, button_base_width]);
}

module button_top_neg() {
    translate([0, 0, button_neg_z_pos])
    sphere(r=button_top_neg_radius);
}

translate([21, 0, 0])
difference() {
    button_body();
    button_crossbar();
    button_top_neg();
}

