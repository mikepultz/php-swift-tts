PHP_ARG_ENABLE(swift_tts,
	[Whether to enable the "swift_tts" extension],
	[  --enable-swift-tts      Enable "swift_tts" extension support])

if test -z "$PHP_LAME_DIR"; then
	PHP_ARG_WITH(lame-dir, for the location of mp3lame,
	[  --with-lame-dir[=DIR]     Swift: Set the path to mp3lame install prefix], no, no)
fi
if test -z "$PHP_GSM_DIR"; then
	PHP_ARG_WITH(gsm-dir, for the location of libgsm,
	[  --with-gsm-dir[=DIR]     Swift: Set the path to libgsm install prefix], no, no)
fi

AC_DEFUN([PHP_SWIFT_LAME],[
	if test "$PHP_LAME_DIR" != "no"; then

    		for i in $PHP_LAME_DIR /usr/local /usr; do
      			test -f $i/$PHP_LIBDIR/libmp3lame.$SHLIB_SUFFIX_NAME || test -f $i/$PHP_LIBDIR/libmp3lame.a && SWIFT_LAME_DIR=$i && break
    		done

    		if test -z "$SWIFT_LAME_DIR"; then
      			AC_MSG_ERROR([libmp3lame.(a|so) not found.])
		else
			PHP_ADD_INCLUDE($SWIFT_LAME_DIR/include/lame)
			PHP_ADD_LIBRARY_WITH_PATH(mp3lame, $PHP_LAME_DIR, SWIFT_TTS_SHARED_LIBADD)
			AC_DEFINE(HAVE_SWIFT_LAME, 1, [ ])
    		fi
  	else
    		AC_MSG_RESULT([If configure fails try --with-lame-dir=<DIR>])   
  	fi
])

AC_DEFUN([PHP_SWIFT_GSM],[
	if test "$PHP_GSM_DIR" != "no"; then

    		for i in $PHP_GSM_DIR /usr/local /usr; do
      			test -f $i/$PHP_LIBDIR/libgsm.$SHLIB_SUFFIX_NAME || test -f $i/$PHP_LIBDIR/libgsm.a || 
				test -f $/libgsm.$SHLIB_SUFFIX_NAME || test -f $i/libgsm.a && SWIFT_GSM_DIR=$i && break
    		done

    		if test -z "$SWIFT_GSM_DIR"; then
      			AC_MSG_ERROR([libgsm.(a|so) not found.])
		else
			PHP_ADD_INCLUDE($SWIFT_GSM_DIR/include)
			PHP_ADD_INCLUDE($SWIFT_GSM_DIR)
			PHP_ADD_LIBRARY_WITH_PATH(gsm, $PHP_GSM_DIR, SWIFT_TTS_SHARED_LIBADD)
			AC_DEFINE(HAVE_SWIFT_GSM, 1, [ ])
    		fi
  	else
    		AC_MSG_RESULT([If configure fails try --with-gsm-dir=<DIR>])   
  	fi
])

if test $PHP_SWIFT_TTS != "no"; then

	PHP_REQUIRE_CXX()
	PHP_SUBST(SWIFT_TTS_SHARED_LIBADD)
	PHP_ADD_INCLUDE(/opt/swift/include)
	PHP_ADD_LIBRARY(stdc++, 1, SWIFT_TTS_SHARED_LIBADD)
	PHP_ADD_LIBRARY_WITH_PATH(swift, /opt/swift/lib, SWIFT_TTS_SHARED_LIBADD)
	PHP_ADD_LIBRARY_WITH_PATH(ceplang_en, /opt/swift/lib, SWIFT_TTS_SHARED_LIBADD)
	PHP_ADD_LIBRARY_WITH_PATH(ceplex_us, /opt/swift/lib, SWIFT_TTS_SHARED_LIBADD)
	PHP_NEW_EXTENSION(swift_tts, cg711.cc cswift.cc php_swift.cc, $ext_shared)

	PHP_SWIFT_LAME
	PHP_SWIFT_GSM
fi
