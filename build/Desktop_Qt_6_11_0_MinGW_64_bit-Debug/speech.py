import speech_recognition as sr
import sys

def listen_voice():
    r = sr.Recognizer()
    try:
        with sr.Microphone() as source:
            # Adaptasi noise
            r.adjust_for_ambient_noise(source, duration=1)
            audio = r.listen(source, timeout=8, phrase_time_limit=8)
            text = r.recognize_google(audio, language="id-ID")
            return text
    except sr.WaitTimeoutError:
        return "ERROR:Tidak ada suara terdeteksi"
    except sr.UnknownValueError:
        return "ERROR:Suara tidak dikenali"
    except sr.RequestError:
        return "ERROR:Koneksi ke Google Speech gagal"
    except Exception as e:
        return f"ERROR:{str(e)}"

result = listen_voice()
# Format output agar mudah di-parse oleh Qt
print(f"RESULT:{result}", flush=True)
sys.exit(0)