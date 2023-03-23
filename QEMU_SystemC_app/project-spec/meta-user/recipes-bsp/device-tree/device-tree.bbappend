FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI += "file://system-user.dtsi"

SRC_URI_append += "file://zynqmp-fpga.dtsi"
