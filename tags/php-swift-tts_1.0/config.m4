PHP_ARG_WITH(swift_tts, for the location of libswift,
[  --with-swift_tts[=DIR]    Enable "swift_tts" extension support])

PHP_ARG_WITH(lame-dir, for the location of mp3lame,
[  --with-lame-dir[=DIR]     Swift: Set the path to mp3lame install prefix], no, no)

PHP_ARG_WITH(gsm-dir, for the location of libgsm,
[  --with-gsm-dir[=DIR]      Swift: Set the path to libgsm install prefix], no, no)

if test $PHP_SWIFT_TTS != "no"; then
	if test $PHP_SWIFT_TTS == "yes"; then
		AC_MSG_ERROR([You must specify a path to libswift when using --with-swift-tts])
	fi

	if test -f $PHP_SWIFT_TTS/$PHP_LIBDIR/libswift.$SHLIB_SUFFIX_NAME; then

		PHP_REQUIRE_CXX()
		PHP_SUBST(SWIFT_TTS_SHARED_LIBADD)
		PHP_ADD_INCLUDE($PHP_SWIFT_TTS/include)
		PHP_ADD_LIBRARY(stdc++, 1, SWIFT_TTS_SHARED_LIBADD)
		PHP_ADD_LIBRARY_WITH_PATH(swift, $PHP_SWIFT_TTS/lib, SWIFT_TTS_SHARED_LIBADD)
		PHP_ADD_LIBRARY_WITH_PATH(ceplang_en, $PHP_SWIFT_TTS/lib, SWIFT_TTS_SHARED_LIBADD)
		PHP_ADD_LIBRARY_WITH_PATH(ceplex_us, $PHP_SWIFT_TTS/lib, SWIFT_TTS_SHARED_LIBADD)
		PHP_NEW_EXTENSION(swift_tts, cg711.cc cswift.cc php_swift.cc, $ext_shared)

	else
		AC_MSG_ERROR([libswift.so was not found in $PHP_SWIFT_TTS/$PHP_LIBDIR]);
	fi

	if test "$PHP_LAME_DIR" != "no"; then
		if test $PHP_LAME_DIR == "yes"; then
			AC_MSG_ERROR([You must specify a path to libmp3lame when using --with-lame-dir])
		fi

		if test -f $PHP_LAME_DIR/$PHP_LIBDIR/libmp3lame.$SHLIB_SUFFIX_NAME || test -f $PHP_LAME_DIR/$PHP_LIBDIR/libmp3lame.a; then

			PHP_ADD_INCLUDE($PHP_LAME_DIR/include/lame)
			PHP_ADD_LIBRARY_WITH_PATH(mp3lame, $PHP_LAME_DIR, SWIFT_TTS_SHARED_LIBADD)
			AC_DEFINE(HAVE_SWIFT_LAME, 1, [Define to 1 to have support for the mp3 LAME encoder.])

		else
      			AC_MSG_ERROR([libmp3lame.(a|so) not found.])
    		fi
  	fi

	if test "$PHP_GSM_DIR" != "no"; then
		if test $PHP_GSM_DIR == "yes"; then
			AC_MSG_ERROR([You must specify a path to libgsm.so when using --with-gsm-dir])
		fi

		if test -f $PHP_GSM_DIR/$PHP_LIBDIR/libgsm.$SHLIB_SUFFIX_NAME || test -f $PHP_GSM_DIR/$PHP_LIBDIR/libgsm.a ||
			test -f $PHP_GSM_DIR/libgsm.$SHLIB_SUFFIX_NAME || test -f $PHP_GSM_DIR/libgsm.a; then

			PHP_ADD_INCLUDE($PHP_GSM_DIR/include)
			PHP_ADD_INCLUDE($PHP_GSM_DIR)
			PHP_ADD_LIBRARY_WITH_PATH(gsm, $PHP_GSM_DIR, SWIFT_TTS_SHARED_LIBADD)
			AC_DEFINE(HAVE_SWIFT_GSM, 1, [Define to 1 to have support for the gsm encoder.])

		else
      			AC_MSG_ERROR([libgsm.(a|so) not found.])
    		fi
  	fi
fi
