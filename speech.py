import speech_recognition as sr

r = sr.Recognizer()

with sr.Microphone() as source:
    print("Listening...")
    audio = r.listen(source)

try:
    text = r.recognize_google(audio, language="id-ID")
    print(text)

except:
    print("")