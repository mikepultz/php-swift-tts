<?php

    $s = new SwiftTTS();

    print_r($s->getVoices());

    if ($s->setVoice("Allison-8kHz") !== false)
    {
        $st = $s->generate("hello my name is allison", SwiftTTS::FORMAT_WAV);
        if ($st !== FALSE)
        {
            $out = stream_get_contents($st);
        
            echo "read " . strlen($out) . " bytes";

            $fp = fopen("test.wav", "w");
            if ($fp)
            {
                fwrite($fp, $out);
                fclose($fp);
            }
        }
    }

?>
