import wave
import numpy as np
import os
import matplotlib.pyplot as plt
from scipy.io import wavfile
import librosa


#C:/Users/sfkap/Code/research/Tobacco Cut/
#https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.spectrogram.html
#https://en.wikipedia.org/wiki/WAV
#https://docs.python.org/3/library/wave.html
max_files = 5
fig, axes = plt.subplots(max_files, 1, figsize=(10, 4 * max_files))

def getfilepath():
    return input("specify file path to wav file: ")

dir = getfilepath()
files = os.listdir(dir)
count = 0
for file in files:
    if count<max_files:
        # Load the WAV file
        sample_rate, data = wavfile.read(str(dir)+"/"+str(file))


        time = np.linspace(0, len(data) / sample_rate, num=len(data))


        # Plot the waveform
        axes[count].plot(time, data)
        axes[count].set_title(f'Waveform of {file}')
        axes[count].set_xlabel('Time [s]')
        axes[count].set_ylabel('Amplitude')
        axes[count].grid(True)
        count +=1
        
    else:
        break


plt.tight_layout()
plt.show()