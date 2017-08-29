# Linphone on MacOS X

## Build prerequisite

* Xcode (download from apple or using appstore application)
* [Java SE](http://www.oracle.com/technetwork/java/javase/downloads/index.html) or openJDK
 This is required to generate a C sourcefile from SIP grammar using [antlr3](http://www.antlr3.org/) generator.
* [HomeBrew](http://brew.sh) or [Macports](http://www.macports.org/).

### Dependencies

#### Using MacPorts

##### Multiple MacOS version support

In order to enable generation of bundle for older MacOS version, it is recommended to:

 Edit `/opt/local/etc/macports/macports.conf` to add the following line:

 > macosx_deployment_target 10.7
 > buildfromsource always


##### Third party libraries

        sudo port install automake autoconf libtool pkgconfig intltool wget bcunit \
        antlr3 speex readline sqlite3 openldap libupnp \
        ffmpeg-devel -gpl2

##### Install libantlr3c
Library used by belle-sip for parsing.

        git clone -b linphone git://git.linphone.org/antlr3.git
        cd antlr3/runtime/C
        ./autogen.sh
        ./configure --disable-static --prefix=/opt/local && make
        sudo make install

##### Install libvpx
Must be manually build because the macport recipe does not support 'macosx_deployment_target'.

        git clone https://chromium.googlesource.com/webm/libvpx -b v1.4.0
        cd libvpx
        ./configure --prefix=/opt/local \
                --target=x86_64-darwin10-gcc \
                --enable-error-concealment \
                --enable-multithread \
                --enable-realtime-only \
                --enable-spatial-resampling \
                --enable-vp8 \
                --disable-vp9 \
                --enable-libs \
                --disable-install-docs \
                --disable-debug-libs \
                --disable-examples \
                --disable-unit-tests \
                --as=yasm
        make
        sudo make install
	
##### Linphone UI (GTK version)

Install `GTK`. It is recommended to use the `quartz` backend for better integration.

        sudo port install gtk2 +quartz +no_x11
        sudo port install gtk-osx-application-gtk2 +no_python
        sudo port install hicolor-icon-theme

#### Using HomeBrew

##### Third party libraries

        brew install intltool libtool wget pkg-config automake libantlr3c git \
             gettext speex ffmpeg readline libvpx opus yasm openh264
        brew install gtk+ cairo gtk-mac-integration hicolor-icon-theme --without-x11
	sudo easy_install pip
	sudo pip install pystache
	
### Check out Linphone source

	git clone https://github.com/BelledonneCommunications/linphone-desktop
	cd linphone-desktop

### Building
First we need pip and a python module:

	export Qt5_DIR=/usr/local/Cellar/qt/5.9.1/lib/cmake/

If you want compatibility with old OSX versions:

        export MACOSX_DEPLOYMENT_TARGET=10.7
        export LDFLAGS="-Wl,-headerpad_max_install_names"

	./prepare.py --all-codecs --package
	make

