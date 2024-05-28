import wave
import numpy as np
from scipy import signal
import matplotlib.pyplot as plt



def getfilepath():
    return input("specify file path to wav file")

#extract wave into a numpy array
signal_wave = wave.open(getfilepath(), 'r')
sample_frequency = signal_wave.getframerate()

data = np.fromstring(signal_wave.readframes(sample_frequency), dtype=np.int16)
sig = signal_wave.readframes(-1)

sig = np.fromstring(sig, 'Int16')
#sig = sig[:]



#lot the data
plt.figure(1)
c = plt.subplot(211)
Pxx, freqs, bins, im = c.specgram(sig, NFFT=1024, Fs=sample_frequency, noverlap=900)
c.set_xlabel('Time')
c.set_ylabel('Frequency')
plt.show()