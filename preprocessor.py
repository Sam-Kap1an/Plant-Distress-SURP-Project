import wave
import numpy as np
import matplotlib.pyplot as plt
from scipy.io import wavfile

#https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.spectrogram.html
#https://en.wikipedia.org/wiki/WAV
#https://docs.python.org/3/library/wave.html

def getfilepath():
    return input("specify file path to wav file ")

# Load the WAV file
sample_rate, data = wavfile.read(getfilepath())

time = np.linspace(0, len(data) / sample_rate, num=len(data))
    
# Plot the waveform
plt.figure(figsize=(10, 4))
plt.plot(time, data)
plt.title('Waveform')
plt.xlabel('Time [s]')
plt.ylabel('Amplitude')
plt.grid(True)
plt.show()