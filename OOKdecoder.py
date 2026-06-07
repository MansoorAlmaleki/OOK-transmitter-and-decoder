import numpy as np
import pygame
import sys
from scipy.io import wavfile
#the transmitter uses a fixed bit width on-off keying encoding method
#audio file crop must be alligned with the first bit of the preamble in order to work and a little longer than the postamble to make sure data does
#not get cropped out

sample_rate, audio_data = wavfile.read('AudioData/25msBitWidth32x32BinaryImageA.wav')
bit_width = 0.025
samples_per_bit = bit_width * sample_rate
image_size = 32*32
preamble_size = 8
postamble_size = 8
total_samples_of_data = samples_per_bit*(image_size+preamble_size+postamble_size)#this is a constant where the data is 1024 bits plus 16 for preamble and postamble
threshold = 0.075#tune this value, this is the bit detection threshold which checks if the intensity is greater than or equal to threshold to append it as an on or off

BinaryData = []
Index = 0
ProcessingAudio = True

float_data = audio_data.astype(np.float32)
abs_data = np.abs(float_data)
normalized_data = abs_data/np.max(abs_data)

while ProcessingAudio:
    if(Index+samples_per_bit <= total_samples_of_data):
        Intensity = np.mean(normalized_data[Index:Index+samples_per_bit])
        if(Intensity > threshold):
            BinaryData.append(1)
            #print("1")
        else:
            #print("0")
            BinaryData.append(0)
    else:
        break
    Index += samples_per_bit

BinaryData = BinaryData[preamble_size:-postamble_size]#remove the preamble and the postamble so the only data left is the image data

pygame.init()
screen = pygame.display.set_mode((704, 704))

while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()

    screen.fill((0, 0, 0))
    for y in range(32):
        x = 0
        for x in range(32):
            if(BinaryData[(y*32)+x] == 1):
                pygame.draw.rect(screen, (255,255,255), (x*22,y*22,22,22))
    y = 0
    pygame.display.flip()