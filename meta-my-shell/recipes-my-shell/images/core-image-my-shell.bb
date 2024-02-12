inherit core-image
CORE_IMAGE_EXTRA_INSTALL += "my-shell"

inherit extrausers
# See https://docs.yoctoproject.org/singleindex.html#extrausers-bbclass
# set my-shell as the default shell, that's will make change in the passwd file

EXTRA_USERS_PARAMS = "usermod -s ${bindir}/my-shell root;"
