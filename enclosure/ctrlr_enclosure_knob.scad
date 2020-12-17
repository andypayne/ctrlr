$fn = 100;

// All in mm

threaded_shaft_diam    = 7.1;  // Not used, too low
threadless_shaft_diam  = 6.0;
notch_pos_thickness    = 4.8;
notched_shaft_height   = 10.5;
notchless_shaft_height = 3.3;
top_thickness          = 5.0;
outer_cyl_height       = notched_shaft_height + notchless_shaft_height + top_thickness;
outer_cyl_diam         = 54.0;
shaft_hole_diam        = 6.0;
notch_neg_width        = threadless_shaft_diam - notch_pos_thickness;

neg_inner_cyl_height = outer_cyl_height;
neg_inner_cyl_diam   = outer_cyl_diam - 10.0;
inner_post_overhang  = 1.0;
inner_post_height    = outer_cyl_height + inner_post_overhang;
inner_post_diam      = 12.0;
shaft_hole_height    = inner_post_height;

// Notch in the shaft negative
color("red")
translate([0.5 * threadless_shaft_diam - notch_neg_width, -0.5 * threadless_shaft_diam, notchless_shaft_height])
cube([notch_neg_width, threadless_shaft_diam, notched_shaft_height]);

intersection() {
    intersection() {
        // Negative cone for chamfer top
        translate([0, 0, -50]) cylinder(h=120, d1=120, d2=1);
        difference() {
            cylinder(h=outer_cyl_height, d=outer_cyl_diam);
            translate([0, 0, -top_thickness])
                cylinder(h=neg_inner_cyl_height, d=neg_inner_cyl_diam);
        }
    }

    // Negative cone for chamfer bottom
    rotate([180, 0, 0]) translate([0, 0, -69]) cylinder(h=120, d1=120, d2=1);
}


difference() {
    translate([0, 0, -inner_post_overhang])
        cylinder(h=inner_post_height, d=inner_post_diam);
    translate([0, 0, -inner_post_overhang - 3.0])
        cylinder(h=shaft_hole_height, d=shaft_hole_diam);
}
