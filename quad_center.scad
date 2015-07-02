
// mounting pegs for pi a+
module pi_mount() {
  translate([0, 0, 0]) difference() {
    hull() {
      translate([0, 0, 2]) cylinder(r=3, h=3);
      cylinder(r=2.5, h=5);
    };
    cylinder(r=1.5, h=5);
  };
  translate([49, 0, 0]) difference() {
    hull() {
      translate([0, 0, 2]) cylinder(r=3, h=3);
      cylinder(r=2.5, h=5);
    };
    cylinder(r=1.5, h=5);
  };
  translate([0, 58, 0]) difference() {
    hull() {
      translate([0, 0, 2]) cylinder(r=3, h=3);
      cylinder(r=2.5, h=5);
    };
    cylinder(r=1.5, h=5);
  };
  translate([49, 58, 0]) difference() {
    hull() {
      translate([0, 0, 2]) cylinder(r=3, h=3);
      cylinder(r=2.5, h=5);
    };
    cylinder(r=1.5, h=5);
  };
};

// body piece for each layer
module body_layer() {
  difference() {
    cube([64, 64, 3]);
    union() {

      // the hole pattern
      translate([32, 32, 0]) cylinder(r=18, h=5);
      translate([8, 32, 0]) union() {
        translate([-8, -8, 0]) cube([16, 16, 5]);
        translate([-2, 12, 0]) cylinder(r=1.75, h=5);
        translate([-2, -12, 0]) cylinder(r=1.75, h=5);
      };
      translate([32, 8, 0]) union() {
        translate([-8, -8, 0]) cube([16, 16, 5]);
        translate([12, -2, 0]) cylinder(r=1.75, h=5);
        translate([-12, -2, 0]) cylinder(r=1.75, h=5);
      };
      translate([64-8, 32, 0]) union() {
        translate([-8, -8, 0]) cube([16, 16, 5]);
        translate([2, 12, 0]) cylinder(r=1.75, h=5);
        translate([2, -12, 0]) cylinder(r=1.75, h=5);
      };
      translate([32, 64-8, 0]) union() {
        translate([-8, -8, 0]) cube([16, 16, 5]);
        translate([12, 2, 0]) cylinder(r=1.75, h=5);
        translate([-12, 2, 0]) cylinder(r=1.75, h=5);
      };

      // holes for the pi mount
      translate([7, 3, 0]) union() {
        translate([0, 0, 0]) cylinder(r=1.5, h=5);
        translate([49, 0, 0]) cylinder(r=1.5, h=5);
        translate([0, 58, 0]) cylinder(r=1.5, h=5);
        translate([49, 58, 0]) cylinder(r=1.5, h=5);
      };
    };
  };
};

// one of the quad arms
module arm() {
  difference() {
    hull() {
      cube([32, 10, 3]);
      translate([16, 50, 0]) cylinder(r=12, h=2);
    };
    hull() {
      translate([8, 0, 0]) cube([16, 10, 5]);
      translate([16, 50, 0]) cylinder(r=8, h=5);
    };
  };
};


// ============
// the assembly
// ============

module pi_layer() {
  // main body layer
  translate([0, 0, 0]) body_layer();

  // arm
  translate([16, 64, 0]) rotate([0, 0, 000]) arm(); // protudes +y
  translate([48, 00, 0]) rotate([0, 0, 180]) arm(); // protudes -y
  translate([00, 16, 0]) rotate([0, 0, 090]) arm(); // protudes +x
  translate([64, 48, 0]) rotate([0, 0, 270]) arm(); // protudes -x

  // pi mount
  //translate([7, 3, 2]) pi_mount();
};
pi_layer();

