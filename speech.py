import speech_recognition as sr
import time

r = sr.Recognizer()

try:
    with sr.Microphone() as source:

        print("Silakan bicara...")

        # tunggu mic stabil
        r.adjust_for_ambient_noise(source, duration=1)

        # kasih waktu user mulai bicara
        time.sleep(1)

        # dengarkan suara
        audio = r.listen(
            source,
            timeout=5,
            phrase_time_limit=5
        )

        text = r.recognize_google(
            audio,
            language="id-ID"
        )

        print(text)

except sr.WaitTimeoutError:
    print("Tidak ada suara")

except sr.UnknownValueError:
    print("Suara tidak dikenali")

except Exception as e:
    print(str(e))