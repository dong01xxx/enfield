OPENQASM 2.0;
include "qelib1.inc";

qreg r[5];

cx r[4], r[0];
cx r[4], r[3];
cx r[0], r[3];
cx r[1], r[3];
cx r[1], r[2];
cx r[2], r[3];
cx r[1], r[4];
