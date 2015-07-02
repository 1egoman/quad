
// the arm mount hole pattern
module arm_mount(h) {
  cylinder(r=4, h=h);
  translate([-2, 12, 0]) cylinder(r=1.75, h=h);
  translate([-2, -12, 0]) cylinder(r=1.75, h=h);
};

module arm() {
  difference() {
    hull() {
      cylinder(r=20, h=5);
      translate([100, 0, 0]) cylinder(r=10, h=5);
    }
    union() {
      arm_mount(5);
      hull() {
        translate([22.5, 0, 0]) cylinder(r=10, h=5);
        translate([80, 0, 0]) cylinder(r=6, h=5);
      };
    };
  };
};

arm();