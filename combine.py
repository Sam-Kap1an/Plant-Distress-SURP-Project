# creates a wav file made up of multiple input files
# goal: need to overlay (the times should match)
import wave
import numpy as np
import matplotlib.pyplot as plt
from scipy.io import wavfile
from pydub import AudioSegment

def getfilepath():
    return input("specify file path to wav file ")

sound1 = AudioSegment.from_file(getfilepath())
sound2 = AudioSegment.from_file(getfilepath())

played_together = sound1.overlay(sound2)

output_file = "output.wav"

played_together.export(output_file, format="wav")