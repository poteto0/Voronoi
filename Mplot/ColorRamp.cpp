#include<vector>

class ColorRamp
{
private:
    int type = 2;
    std::vector<float> col1;
    std::vector<float> col2;
    std::vector<float> col3;
public:
    ColorRamp(const std::vector<float> from, const std::vector<float> to);
    ColorRamp(const std::vector<float> from, const std::vector<float> via, const std::vector<float> to);
    ~ColorRamp();

    std::vector<float> color = {0,0,0};

    void update(const float value, const float min, const float max){
        float value_scaled = (value - min) / (max - min);
        if (type == 2)
        {
        
            for (int i = 0; i < 3; i++)
            {
                if(value_scaled < 0){
                    color[i] = col1[i];
                } else if (value_scaled < 1) {
                    color[i] = col1[i] + value_scaled * (col2[i] - col1[i]);
                } else {
                    color[i] = col2[i];
                }
            }
        }
        else if (type == 3)
        {
            for (int i = 0; i < 3; i++)
            {
                if(value_scaled < 0){
                    color[i] = col1[i];
                } else if (value_scaled < 0.5) {
                    color[i] = col1[i] + 2 * value_scaled * (col2[i] - col1[i]);
                } else if (value_scaled < 1) {
                    color[i] = col2[i] + 2 * value_scaled * (col3[i] - col2[i]);
                } else {
                    color[i] = col3[i];
                }
            }
        }
    } 
};

ColorRamp::ColorRamp(const std::vector<float> from, const std::vector<float> to)
{
    type = 2;
    col1 = from;
    col2 = to;
}

ColorRamp::ColorRamp(const std::vector<float> from, const std::vector<float> via, const std::vector<float> to)
{
    type = 3;
    col1 = from;
    col2 = via;
    col3 = to;
}

ColorRamp::~ColorRamp()
{
}