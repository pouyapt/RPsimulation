#ifndef generators_h
#define generators_h

namespace core {

static std::random_device ran;

class random {
private:
    std::string set1[27] = {
        "b","c","d","f","g","h","j","k","l","m","n","p","q","r","s","t","v","w","x","y","z","ch","sh","kh","gh","zh","ph"
    };
    float set1_px[27] = {
        0.065,0.01,0.059,0.055,0.054,0.050,0.046,0.041,0.048,0.065,0.062,0.057,0.007,0.078,0.079,0.072,0.011,0.022,0.002,0.015,0.012,0.025,0.036,0.015,0.009,0.002,0.003
    };
    const std::string set2[5] = {
        "a","e","i","o","u"
    };
    float set2_px[5] = {
        0.31,0.24,0.2,0.19,0.06
    };
    int string_length[10] = {
        3,4,5,6,7,8,9,10,11,12
    };
    float string_lengh_px[10] = {
        0.07,0.244,0.29,0.19,0.14,0.045,0.008,0.009,0.003,0.001
    };
    std::string get_consonant ()
    {
        std::string consonant;
        float low_set1_fx;
        float high_set1_fx;
        float ran_number;
        low_set1_fx = 0;
        high_set1_fx = set1_px[0];
        ran_number = (ran()%1000+1)*0.001;
        for (int i=0; i<27; i++)
        {
            if (ran_number > low_set1_fx && ran_number <= high_set1_fx)
            {
                consonant = set1[i];
                break;
            }
            else
            {
                low_set1_fx = low_set1_fx + set1_px[i];
                high_set1_fx = high_set1_fx + set1_px[i+1];
            }
        }
        return consonant;
    }
    std::string get_vowel ()
    {
        std::string vowel;
        float low_set2_fx;
        float high_set2_fx;
        float ran_number;
        low_set2_fx = 0;
        high_set2_fx = set2_px[0];
        ran_number = (ran()%1000+1)*0.001;
        for (int i=0; i<5; i++)
        {
            if (ran_number > low_set2_fx && ran_number <= high_set2_fx)
            {
                vowel = set2[i];
                break;
            }
            else
            {
                low_set2_fx = low_set2_fx + set2_px[i];
                high_set2_fx = high_set2_fx + set2_px[i+1];
            }
        }
        return vowel;
    }
    int get_length ()
    {
        int length = 0;
        float low_length_fx = 0;
        float high_length_fx = string_lengh_px[0];
        float ran_number;
        ran_number = (ran()%1000+1)*0.001;
        for (int i=0; i<10; i++)
        {
            if (ran_number > low_length_fx && ran_number <= high_length_fx)
            {
                length = string_length[i];
                break;
            }
            else
            {
                low_length_fx = low_length_fx + string_lengh_px[i];
                high_length_fx = high_length_fx + string_lengh_px[i+1];
            }
        }
        return length;
    }
    void name_template (int length, int* temp)
    {
        int digit;
        float ran_number;
        for (int i=0; i<length; i++)
        {
            ran_number = (ran()%1000+1)*0.001;
            if (ran_number <= 0.75)
                digit = 1;
            else
                digit = 0;
            if (i>=2 && temp[i-1] == temp[i-2])
                temp[i] = (~temp[i-1])&1;
            else
                temp[i] = digit;
            if (i==0)
            {
                temp[1] = (~temp[0])&1;;
                i++;
            }
            if (i==length-2 && temp[i]==1)
            {
                temp[i+1] = 0;
                i++;
            }
        }
    }
public:
    long id_number(int lowBound, int highBound) {
        std::mt19937 eng(ran());
        std::uniform_int_distribution<> distr(lowBound, highBound);
        return distr(eng);
    }
    auto string_name() {
        int length = get_length();
        int temp[length];
        std::string name;
        name_template (length, temp);
        for (int i=0; i<length; i++)
        {
            if (temp[i] == 1)
                name = name + get_consonant();
            else
                name = name + get_vowel();
        }
        name[0] = toupper(name[0]);
        return name;
    }
    auto minig_power() {
        std::mt19937 gen{ran()};
        std::normal_distribution<> d(50, 20);
        double a = -1;
        while (a<=6.5)
            a = d(gen);
        return a;
    }
    auto dishonestyFactor() {
        std::mt19937 gen{ran()};
        std::normal_distribution<> d(0, 3);
        long double a = -1;
        while (a<0)
            a = d(gen);
        return a;
    }
};

}


#endif /* generators_h */
