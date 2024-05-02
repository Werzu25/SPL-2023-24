class Audio
{
private:
    unsigned long milion;
    unsigned long previousMillis = 0;
    int audioPin;
    int audio;
public:
    Audio(int audioPin)
    {
        this->audioPin = audioPin;
        pinMode(audioPin, OUTPUT);
    }
    void Audio::startAudio(int audio)
    {
        tone(audioPin, audio);
          
    }
    void Audio::stopAudio()
    {
        noTone(audioPin);
    }   
};
