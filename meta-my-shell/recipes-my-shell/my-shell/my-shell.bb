# Recipe created by recipetool
# This is the basis of a recipe and may need further editing in order to be fully functional.
# (Feel free to remove these comments when editing.)

# WARNING: the following LICENSE and LIC_FILES_CHKSUM values are best guesses - it is
# your responsibility to verify that the values are complete and correct.
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://Makefile \
           file://my-shell.c \
           file://my-shell.h \
          "

S = "${WORKDIR}"

# NOTE: this is a Makefile-only piece of software, so we cannot generate much of the
# recipe automatically - you will need to examine the Makefile yourself and ensure
# that the appropriate arguments are passed in.

TARGET_CC_ARCH += "${LDFLAGS}"

do_compile () {
	# You will almost certainly need to add additional arguments here
	oe_runmake
}

do_install () {

	install -d ${D}${bindir}
    install -m 0755 ${S}/my-shell ${D}${bindir}
}


