//
// Created by solovyev.an on 5/29/2017.
//

#ifndef MOVINGAVERAGE_MOVINGAVERAGE_H
#define MOVINGAVERAGE_MOVINGAVERAGE_H

#include <vector>
#include <iostream>
#include "cpp_support_fracDiff/TImeSeriesUtilsMajorFunctions.h"


//vector length alignment
void alignVectorLength_(std::vector<double>& MA_vector, const unsigned int dataVectorLengthInt_);

//rolling window for all calcs
void getRollingWindow(std::vector<double>& dataVector, std::vector<double>& maDataVector,
                      const unsigned int dataVectorLengthInt_,
                           const unsigned int MA_length, const unsigned int pos);
//getting first difference for data prep
std::vector<double> diff(const std::vector<double>& dataVector, unsigned int lag = 1);


class SMA
{
public:
    //ctors
    //SMAs
    SMA(unsigned int nSlow, unsigned int nFast, const std::vector<double>& data);

    //dtor
    ~SMA();

    //resulting SMAs
    std::vector<double> SMA_slow;
    std::vector<double> SMA_fast;

    //IB flag
    bool IB;

private:
    //data
    std::vector<double>* data_; //Close price, usually
    unsigned int dataVectorLengthInt_;

    //two SMAs
    void calculateSMA_();
    //SMAa params
    unsigned int nSMA_slow_;
    unsigned int nSMA_fast_;

    //nuts and bolts
    //SMA
    std::vector<double> _SMA_(const unsigned int MA_length);

};

class EMA
{
public:
    //ctors
    //EMA
    EMA(unsigned int nEMA, const std::vector<double>& data);

    //dtor
    ~EMA();

    //resulting EMA
    std::vector<double> EMAs;

    //IB flag
    bool IB;

private:
    //data
    std::vector<double>* data_; //Close price, usually
    unsigned int dataVectorLenghtInt_;

    //EMA calcs
    void calculateEMA_();
    //EMA params
    unsigned int nEMA_;

    //nuts and bolts
    //EMA weighted with alpha
    std::vector<double> _EMA_(const unsigned int MA_length);

};

class WMA
{
public:
    //ctor WMA
    WMA(unsigned int nWMA, const std::vector<double>& price, const std::vector<double>& weight);

    //dtor
    ~WMA();

    //resulting WMA
    std::vector<double> WMAs;

    //IB flag
    bool IB;

private:
    //data
    std::vector<double>* data_; //Close price, usually
    std::vector<double>* weight_; //weights
    unsigned int dataVectorLengthInt_;

    //WMA
    void calculateWMA_();
    unsigned int nWMA_;

    //nuts and bolts
    //WMA
    std::vector<double> _WMA_(unsigned int nWMA);

};

class MACD
{
public:
    //ctors
    //MACDs
    MACD(unsigned int nSlow, unsigned int nFast, unsigned int nSignal, const std::vector<double>& data);

    //dtor
    ~MACD();

    //resulting MACDs
    std::vector<double> MACD_macd;
    std::vector<double> MACD_signal;
    std::vector<double> MACD_osc;

    //IB flag
    bool IB;

private:
    //data
    std::vector<double>* data_; //Close price, usually
    unsigned int dataVectorLengthInt_;


    //MACD calcs
    void calculateMACD_();
    //MACDa params
    unsigned int nMACD_slow_;
    unsigned int nMACD_fast_;
    unsigned int nMACDSignal_;

    //nuts and bolts
    //EMA weighted with alpha
    std::vector<double> _EMA_(const unsigned int MA_length);

};

class RSI
{
public:
    //ctors
    //default
    RSI(const std::vector<double>& data);
    //SMAs and EMAs
    RSI(std::string maType, unsigned int nMA, const std::vector<double>& data);
    //WMA
    RSI(std::string maType, unsigned int nMA, const std::vector<double>& price, const std::vector<double>& weight);

    //dtor
    ~RSI();

    //resulting RSI on respective MA
    std::vector<double> RSIs;

    //IB flag
    bool IB;

private:
    //data
    std::vector<double>* data_; //Close price, usually
    std::vector<double>* weight_; //weights
    unsigned int dataVectorLengthInt_;
    //data diffs
    std::vector<double> data_firstDiff_;
    std::vector<double> data_diffUp_;
    std::vector<double> data_diffDown_;
    //maType
    std::string maType_;
    //MA length
    unsigned int nMA_;

    //prepare data for RSI
    void calculateRSI_dataPrep_();

    //calculate RSI
    void calculateRSI_SMA_();

    //calculate RSI
    void calculateRSI_EMA_();

    //WMA
    bool typeVWMA;
    //calculate RSI
    void calculateRSI_VWMA_();
};


#endif //MOVINGAVERAGE_MOVINGAVERAGE_H
