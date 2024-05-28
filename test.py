import wave
import numpy as np
from scipy import signal
import matplotlib.pyplot as plt

wav = wave.open("test_sound.wav", "r")

raw = wav.readframes(-1)
raw = np.frombuffer(raw, "int16")

plt.title("Wav File")

plt.plot(raw, color="blue")
plt.ylabel("Amplitude")
plt.xlabel("Time")

plt.show()
