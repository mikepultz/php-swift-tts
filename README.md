# php\_swift\_tts - a PHP extension for the Swift text-to-speech engine. #

### The swift text-to-speech engine is the engine used for the Cepstral Text-to-Speech Voices ###

This extension requires the libswift, libceplang\_en and libceplex\_us librarys included with any Cepstral Text-to-Speech voice, and supports any 16khz and 8khz voices installed on the system.

This extension supports multiple audio output formats, including:

  * PCM (RAW audio)
  * u-law / a-law (logarithmically encoded RAW audio)
  * WAV (RAW audio)
  * GSM (when compiled with the libgsm library)
  * MP3 (when compiled with the libmp3lame library)

### A simple example: ###

```

//
// create the new TTS object
//
$tts = new SwiftTTS();

//
// set a voice to use for generation
//
$tts->setVoice("Allison");

//
// generate text, and return a stream for the audio
//
$s = $tts->generate("hello my name is allison", SwiftTTS::FORMAT_WAV);
if ($s !== false)
{
	//
	// write the stream contents to a file
	//
	file_put_contents("audio.wav", $s);
}

```

### Get an array of available voices: ###
```
//
// create the new TTS object
//
$tts = new SwiftTTS();

//
// get a list of available voices
//
$voices = $tts->getVoices();

//
// show the voices
//
print_r($voices);
```

### this produces: ###
```

Array
(
    [Allison-8kHz] => Array
        (
            [id] => voice
            [path] => /opt/swift/voices/Allison-8kHz
            [version] => 5.1.0
            [buildstamp] => 2008-04-07 15:02:17 EDT <sisyphus@flea>
            [sample_rate] => 8000
            [license_key] =>
            [language_tag] => en-us
            [language_name] => US English
            [language_version] => 0.0.0
            [lexicon_name] => unknown
            [lexicon_version] => 0.0.0
            [speaker_name] => Allison-8kHz
            [speaker_gender] => Female
            [speaker_age] => 28
        )

    [William] => Array
        (
            [id] => voice
            [path] => /opt/swift/voices/William
            [version] => 5.1.0
            [buildstamp] => 2007-10-12 00:39:32 EDT <sisyphus@moth>
            [sample_rate] => 16000
            [license_key] => 
            [language_tag] => en-us
            [language_name] => US English
            [language_version] => 0.0.0
            [lexicon_name] => unknown
            [lexicon_version] => 0.0.0
            [speaker_name] => William
            [speaker_gender] => male
            [speaker_age] => 30
        )
)
```

### Basic Installation ###
```

# cd php-swift-tts
# phpize
# ./configure --with-swift_tts=<path to swift libraries>
# make
# make install

```

### Optional Configure Arguments ###
```

To build with MP3 support
  --with-lame-dir=DIR

To build with GSM support
  --with-gsm-dir=DIR

```
