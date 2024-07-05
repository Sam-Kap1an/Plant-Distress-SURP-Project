import matplotlib.pyplot as plt
from scipy import signal
from scipy.io import wavfile
import numpy as np

def getfilepath():
    return input("Specify file path to wav file: ")

sample_rate, samples = wavfile.read(getfilepath())

# If stereo, convert to mono
if samples.ndim == 2:
    samples = samples.mean(axis=1)

frequencies, times, spectrogram = signal.spectrogram(samples, sample_rate)

# Convert the spectrogram to dB scale
spectrogram_dB = 10 * np.log10(spectrogram + 1e-10)

plt.figure(figsize=(10, 5))
plt.imshow(spectrogram_dB, aspect='auto', extent=[times.min(), times.max(), frequencies.min(), frequencies.max()], origin='lower')
plt.ylabel('Frequency')
plt.xlabel('Time [sec]')
plt.colorbar(label='Intensity [dB]')
plt.show()