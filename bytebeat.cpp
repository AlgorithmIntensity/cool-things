#include <windows.h>
#include <cmath>
#include <iostream>
#include <conio.h>

const int SAMPLE_RATE = 8000;
const int BUFFER_SIZE = 4096;

short your_bytebeat(int t) {
    return (t * t) >> 5;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                   LPSTR lpCmdLine, int nCmdShow) {
    
    HWAVEOUT hWaveOut;
    WAVEFORMATEX waveFormat;
    
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat.nChannels = 1;
    waveFormat.nSamplesPerSec = SAMPLE_RATE;
    waveFormat.nAvgBytesPerSec = SAMPLE_RATE * 2;
    waveFormat.nBlockAlign = 2;
    waveFormat.wBitsPerSample = 16;
    waveFormat.cbSize = 0;
    
    if (waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveFormat, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR) {
        return 1;
    }
    
    WAVEHDR waveHeader;
    short* buffer = new short[BUFFER_SIZE];
    
    waveHeader.lpData = (LPSTR)buffer;
    waveHeader.dwBufferLength = BUFFER_SIZE * 2;
    waveHeader.dwFlags = 0;
    waveHeader.dwLoops = 0;
    
    waveOutPrepareHeader(hWaveOut, &waveHeader, sizeof(WAVEHDR));    
    
    while (true) {
        int t = 0;
        bool playing = true;
        
        while (playing && t < SAMPLE_RATE * 60) {
            for (int i = 0; i < BUFFER_SIZE; i++) {
                buffer[i] = your_bytebeat(t++) * 64;
                
                if (t >= SAMPLE_RATE * 60) {
                    playing = false;
                    for (int j = i + 1; j < BUFFER_SIZE; j++) {
                        buffer[j] = 0;
                    }
                    break;
                }
            }
            
            waveOutWrite(hWaveOut, &waveHeader, sizeof(WAVEHDR));
            
            while (!(waveHeader.dwFlags & WHDR_DONE)) {
                Sleep(1);
            }
        }
    }
    
    waveOutUnprepareHeader(hWaveOut, &waveHeader, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
    delete[] buffer;
    return 0;
}