//
// Created by solovyev.an on 5/29/2017.
//

#include "TechIndicators.h"

#include <iostream>
#include <algorithm>
#include <numeric>
#include <cmath>


//alignes vector up to n length
void alignVectorLength_(std::vector<double>& MA_vector, const unsigned int dataVectorLengthInt_)
{
    //aligning the lengths
    for (int i = MA_vector.size(); i != dataVectorLengthInt_; i++) MA_vector.push_back(0.0);
    //  for (int i = 0; i < MA_vector.size(); i++) if (std::isnan(MA_vector.at(i))) MA_vector.at(i) = 0.0;

    //reversing MA in case it is not IB, that makes the latest the last in vector
    //if (!IB) std::reverse(MA_vector.begin(), MA_vector.end());
}//[!calculateMACD()]

//gets a reversed window, as IB shows reverse order of data
void getRollingWindow(std::vector<double>& dataVector, std::vector<double>& maDataVector,
                      const unsigned int dataVectorLengthInt_,
                      const unsigned int MA_length, const unsigned int pos)
{
    std::copy(dataVector.begin()+ dataVectorLengthInt_ - MA_length - pos,
              dataVector.end()- pos,
              back_inserter(maDataVector));
}


SMA::SMA(unsigned int nSlow, unsigned int nFast, const std::vector<double>& data) :
        nSMA_slow_(nSlow),
        nSMA_fast_(nFast),
        data_( new std::vector<double>(data) )
{
    //in IB data, historicalData is finished with "-1", so I need to skip the very last element of a vector
    dataVectorLengthInt_ = (*data_).size(); //to remove "-1" that finishes data pack in IB
    if (dataVectorLengthInt_ > 1 && (*data_).back() == -1)
    {
        dataVectorLengthInt_ = dataVectorLengthInt_ - 1;
        (*data_).pop_back();
    }

    IB = true;
    calculateSMA_();
}//[!ctor SMA]


SMA::~SMA()
{
	if (data_ != nullptr)
	{
		delete data_;
		data_ = nullptr;
	}
} //[!dtor]

void SMA::calculateSMA_()
{
	SMA_slow = _SMA_(nSMA_slow_);
	SMA_fast = _SMA_(nSMA_fast_);
}//[!calculateSMA()]

std::vector<double> SMA::_SMA_(const unsigned int MA_length)
{
	//temporarily variables
	std::vector<double> temp_;
	std::vector<double> SMA_return_; //SMA_result

    unsigned int pos = 0;

	//getting a vector of slow MAs
	for (int i = dataVectorLengthInt_; i >= MA_length; i--)
    {
		//MA
        getRollingWindow((*data_), temp_, dataVectorLengthInt_, MA_length, pos);
		if (temp_.size() != 0 ) SMA_return_.push_back( std::accumulate(temp_.begin(), temp_.end(), 0.0) / temp_.size() );
		else SMA_return_.push_back(0.0);
        temp_.clear();
        pos++;
	}
	alignVectorLength_(SMA_return_, dataVectorLengthInt_);
	return SMA_return_;
}//[!_SMA_()]



EMA::EMA(unsigned int nEMA, const std::vector<double>& data) :
        nEMA_(nEMA),
        data_( new std::vector<double>(data) )
{
    //in IB data, historicalData is finished with "-1", so I need to skip the very last element of a vector
    dataVectorLenghtInt_ = (*data_).size(); //to remove "-1" that finishes data pack in IB
    if (dataVectorLenghtInt_ > 1 && (*data_).back() == -1)
    {
        dataVectorLenghtInt_ = dataVectorLenghtInt_ - 1;
        (*data_).pop_back();
    }

    IB = true;
    calculateEMA_();
}//[!ctor EMA]

EMA::~EMA()
{
    if (data_ != nullptr)
    {
        delete data_;
        data_ = nullptr;
    }

} //[!dtor]


void EMA::calculateEMA_()
{
    //getting EMAs and Signal
    EMAs = _EMA_(nEMA_);

} //[!calculateEMA()]


std::vector<double> EMA::_EMA_(const unsigned int MA_length)
{

    std::vector<double> temp_; //used to collect the results
    std::vector<double> EMA_return_; //EMA_result

    double data_vector_length_dbl = 0.0;
    double alpha_ = 0.0; //Joshua Ulrich called that standard, don't know why, maybe to check this later

    unsigned int pos = 0;

    //getting a vector of MA
    for (int i = dataVectorLenghtInt_; i >= MA_length; i--)
    {
        getRollingWindow((*data_), temp_, dataVectorLenghtInt_, MA_length, pos);

        data_vector_length_dbl = temp_.size();
        alpha_ = 2 / ((data_vector_length_dbl + 1)); //Joshua Ulrich called that standard, don't know why, maybe to check this later

        //weighting the data
        for (int n = (temp_.size() - 1); n >= 0; n-- )
            temp_.at(n) =  temp_.at(n) * alpha_ * std::pow((1 - alpha_), ((temp_.size() - 1) - n) );

        //getting slow EMA vector
        EMA_return_.push_back( std::accumulate(temp_.begin(), temp_.end(), 0.0)  );

        temp_.clear();
        pos++;
    }

    //aligning the lengths
    alignVectorLength_(EMA_return_, dataVectorLenghtInt_);
    return EMA_return_;
}//[!_EMA_()]



WMA::WMA(unsigned int nWMA, const std::vector<double>& price, const std::vector<double>& weight) :
        nWMA_(nWMA),
        data_( new std::vector<double>(price)),
        weight_( new std::vector<double>(weight))
{
    if ( (*data_).size() != (*weight_).size()) {
        std::range_error e("Lengths of vectors are not equal, can't calculate weights for WMA");
        throw e;
    }

    //in IB data, historicalData is finished with "-1", so I need to skip the very last element of a vector
    dataVectorLengthInt_ = (*data_).size(); //to remove "-1" that finishes data pack in IB
    if (dataVectorLengthInt_ > 1 && (*data_).back() == -1)
    {
        dataVectorLengthInt_ = dataVectorLengthInt_ - 1;
        (*data_).pop_back();
        (*weight_).pop_back();
    }

    IB = true;
    calculateWMA_();
}//[!ctor WMA]

WMA::~WMA()
{
    if (data_ != nullptr)
    {
        delete data_;
        data_ = nullptr;
    }

    if (weight_ != nullptr)
    {
        delete weight_;
        weight_ = nullptr;
    }

} //[!dtor]


void WMA::calculateWMA_()
{
    WMAs = _WMA_(nWMA_);
}//[!calculateWMA()]


std::vector<double> WMA::_WMA_(unsigned int nWMA)
{
    //temporarily variables
    std::vector<double> temp_price, temp_weight;
    std::vector<double> WMA_return_; //WMA_result
    double tempWMA;
    unsigned int pos = 0;

    //getting a vector of slow MAs
    for (int i = dataVectorLengthInt_; i >= nWMA; i--)
    {
        getRollingWindow((*data_), temp_price, dataVectorLengthInt_, nWMA, pos);
        getRollingWindow((*weight_), temp_weight, dataVectorLengthInt_, nWMA, pos);

        //weighting the data
        tempWMA = std::inner_product(temp_price.begin(), temp_price.end(), temp_weight.begin(), 0.0);

        //writing down WMA on this iteration
        WMA_return_.push_back( tempWMA / (std::accumulate(temp_weight.begin(), temp_weight.end(), 0.0) ) );

        temp_price.clear();
        temp_weight.clear();
        pos++;
    }
    alignVectorLength_(WMA_return_, dataVectorLengthInt_);
    return WMA_return_;
}//[!_WMA_()]



MACD::MACD(unsigned int nSlow, unsigned int nFast, unsigned int nSignal, const std::vector<double>& data) :
        nMACD_slow_(nSlow),
        nMACD_fast_(nFast),
        nMACDSignal_(nSignal),
        data_( new std::vector<double>(data) )
{
    //in IB data, historicalData is finished with "-1", so I need to skip the very last element of a vector
    dataVectorLengthInt_ = (*data_).size(); //to remove "-1" that finishes data pack in IB
    if (dataVectorLengthInt_ > 1 && (*data_).back() == -1)
    {
        dataVectorLengthInt_ = dataVectorLengthInt_ - 1;
        (*data_).pop_back();
    }

    IB = true;
    calculateMACD_();
}//[!ctor MACD]


MACD::~MACD()
{
    if (data_ != nullptr)
    {
        delete data_;
        data_ = nullptr;
    }

} //[!dtor]


void MACD::calculateMACD_()
{
    //temporarily variables
    std::vector<double> EMA_fast_, //EMA fast
            EMA_slow_; //EMA slow

    //getting EMAs and Signal
    EMA_slow_ = _EMA_(nMACD_slow_);
    EMA_fast_ = _EMA_(nMACD_fast_);
    MACD_signal = _EMA_(nMACDSignal_);  //writing this directly to public variable

    //writing to resulting vectors
    for (int i = 0; i != dataVectorLengthInt_; i++ )
    {
        MACD_macd.push_back(EMA_fast_.at(i) -  EMA_slow_.at(i));
        MACD_osc.push_back(MACD_macd.at(i) - MACD_signal.at(i));
    }
    //MACD macd and osc vectors are written in here

    // output is contained in:
    // MACD_osc - Oscillator
    // MACD_macd - fastEMA - slowEMA
    // MACD_signal - middleEMA, signalling

} //[!calculateMACD()]

std::vector<double> MACD::_EMA_(const unsigned int MA_length)
{

    std::vector<double> temp_; //used to collect the results
    std::vector<double> EMA_return_; //EMA_result

    double data_vector_length_dbl = 0.0;
    double alpha_ = 0.0; //Joshua Ulrich called that standard, don't know why, maybe to check this later

    unsigned int pos = 0;

    //getting a vector of MA
    for (int i = dataVectorLengthInt_; i >= MA_length; i--)
    {
        getRollingWindow((*data_), temp_, dataVectorLengthInt_, MA_length, pos);

        data_vector_length_dbl = temp_.size();
        alpha_ = 2 / ((data_vector_length_dbl + 1)); //Joshua Ulrich called that standard, don't know why, maybe to check this later

        //weighting the data
        for (int n = (temp_.size() - 1); n >= 0; n-- )
            temp_.at(n) =  temp_.at(n) * alpha_ * std::pow((1 - alpha_), ((temp_.size() - 1) - n) );

        //getting slow EMA vector
        EMA_return_.push_back( std::accumulate(temp_.begin(), temp_.end(), 0.0)  );

        temp_.clear();
        pos++;
    }

    //aligning the lengths
    alignVectorLength_(EMA_return_, dataVectorLengthInt_);
    return EMA_return_;
}//[!_EMA_()]




//default constructor
RSI::RSI(const std::vector<double>& data) :
        data_( new std::vector<double>(data) )
{
    //in IB data, historicalData is finished with "-1", so I need to skip the very last element of a vector
    dataVectorLengthInt_ = (*data_).size(); //to remove "-1" that finishes data pack in IB
    if (dataVectorLengthInt_ > 1 && (*data_).back() == -1)
    {
        dataVectorLengthInt_ = dataVectorLengthInt_ - 1;
        (*data_).pop_back();
    }

    maType_ = "EMA";
    nMA_ = 14;


    IB = true;
    typeVWMA = false;

    calculateRSI_dataPrep_();
    calculateRSI_EMA_();

}//[!ctor SMA and EMA]


RSI::RSI(std::string maType, unsigned int nMA, const std::vector<double>& data) :
        maType_(maType),
        nMA_(nMA),
        data_( new std::vector<double>(data) )
{
    //in IB data, historicalData is finished with "-1", so I need to skip the very last element of a vector
    dataVectorLengthInt_ = (*data_).size(); //to remove "-1" that finishes data pack in IB
    if (dataVectorLengthInt_ > 1 && (*data_).back() == -1)
    {
        dataVectorLengthInt_ = dataVectorLengthInt_ - 1;
        (*data_).pop_back();
    }

    //check maType
//    if (maType_!= "SMA" || maType_!= "sma" || maType_!= "WMA" || maType_!= "wma" || maType_!= "EMA" || maType_!= "ema") {
//        std::invalid_argument e("moving average type to be used in RSI calc is not recognized");
//        throw e;
//    }

    IB = true;
    typeVWMA = false;

    calculateRSI_dataPrep_();

    if (maType_ == "SMA" || maType_ == "sma") calculateRSI_SMA_();
    if (maType_ == "EMA" || maType_ == "ema") calculateRSI_EMA_();

}//[!ctor SMA and EMA]


RSI::RSI(std::string maType, unsigned int nMA, const std::vector<double>& price, const std::vector<double>& weight) :
        maType_(maType),
        nMA_(nMA),
        data_( new std::vector<double>(price)),
        weight_( new std::vector<double>(weight))
{
    if ( (*data_).size() != (*weight_).size()) {
        std::range_error e("Lengths of vectors are not equal, can't calculate weights for WMA");
        throw e;
    }

    //check maType
//    if (maType_!= "SMA" || maType_!= "sma" || maType_!= "WMA" || maType_!= "wma" || maType_!= "EMA" || maType_!= "ema") {
//        std::invalid_argument e("moving average type to be used in RSI calc is not recognized");
//        throw e;
//    }

    //in IB data, historicalData is finished with "-1", so I need to skip the very last element of a vector
    dataVectorLengthInt_ = (*data_).size(); //to remove "-1" that finishes data pack in IB
    if (dataVectorLengthInt_ > 1 && (*data_).back() == -1)
    {
        dataVectorLengthInt_ = dataVectorLengthInt_ - 1;
        (*data_).pop_back();
        (*weight_).pop_back();
    }

    IB = true;
    typeVWMA = true;

    calculateRSI_dataPrep_();
    calculateRSI_VWMA_();
}//[!ctor WMA]

RSI::~RSI()
{
    if (data_ != nullptr)
    {
        delete data_;
        data_ = nullptr;
    }

    if (typeVWMA && weight_ != nullptr)
    {
        delete weight_;
        weight_ = nullptr;
    }

} //[!dtor]


void RSI::calculateRSI_dataPrep_()
{
    //data first difference
    data_firstDiff_ = TimeSeriesUtils::diffVector (*data_, 1);

    //getting positive and negative data
    auto positive = [](double a){
        if (a > 0.0) {
            return a;
        } else {
            return 0.0;
        };
    };
    //negative must be taken as absolute values to follow the index formula
    auto negative = [](double a){
        if (a < 0.0) {
            return -a;
        } else {
            return 0.0;
        };
    };

    std::transform (data_firstDiff_.begin(), data_firstDiff_.end(), back_inserter(data_diffUp_), positive);
    std::transform (data_firstDiff_.begin(), data_firstDiff_.end(), back_inserter(data_diffDown_),negative);
}

void  RSI::calculateRSI_SMA_()
{
    double
            maUp_ = 0.0,
            maDown_ = 0.0;

    std::vector<double> temp_up_, temp_down_;

    unsigned int pos = 0;

    for (int i = dataVectorLengthInt_; i >= nMA_; i--) {

        //assigning sample equal to MA_Length
        getRollingWindow(data_diffUp_, temp_up_, dataVectorLengthInt_, nMA_, pos);
        getRollingWindow(data_diffDown_, temp_down_, dataVectorLengthInt_, nMA_, pos);

        //calculating RSI
        if (temp_up_.size() != 0) maUp_ = std::accumulate(temp_up_.begin(), temp_up_.end(), 0.0) / temp_up_.size();
        else maUp_ = 0.0;
		if (temp_down_.size() != 0) maDown_ = std::accumulate(temp_down_.begin(), temp_down_.end(), 0.0) / temp_down_.size();
		else maDown_ = 0.0;
		if (maDown_ != 0.0) RSIs.push_back((100 - 100 / (1 + maUp_ / maDown_)));
		else RSIs.push_back(0.0);

        //clearing containers, shifting positions
        temp_up_.clear();
        temp_down_.clear();
        pos++;
    }
    alignVectorLength_(RSIs, dataVectorLengthInt_);
}

void  RSI::calculateRSI_EMA_()
{
    double
            maUp_ = 0.0,
            maDown_ = 0.0;

    std::vector<double> temp_up_, temp_down_;

    unsigned int pos = 0;

    double data_vector_length_dbl = 0.0;
    double alpha_ = 0.0; //Joshua Ulrich called that standard, don't know why, maybe to check this later

    for (int i = dataVectorLengthInt_; i >= nMA_; i--)
    {
        getRollingWindow(data_diffUp_, temp_up_, dataVectorLengthInt_, nMA_, pos);
        getRollingWindow(data_diffDown_, temp_down_, dataVectorLengthInt_, nMA_, pos);

        data_vector_length_dbl = temp_up_.size();
        alpha_ = 2 / ((data_vector_length_dbl + 1)); //Joshua Ulrich called that standard, don't know why, maybe to check this later

        //weighting the data
        for (int n = (temp_up_.size() - 1); n >= 0; n-- ) {
            temp_up_.at(n) = temp_up_.at(n) * alpha_ * std::pow((1 - alpha_), ((temp_up_.size() - 1) - n));
            temp_down_.at(n) = temp_down_.at(n) * alpha_ * std::pow((1 - alpha_), ((temp_down_.size() - 1) - n));
        }

        //getting EMA value
		maUp_ = std::accumulate(temp_up_.begin(), temp_up_.end(), 0.0);
		maDown_ = std::accumulate(temp_down_.begin(), temp_down_.end(), 0.0);
		if (maDown_ != 0.0) RSIs.push_back( (100 - 100 / (1 + maUp_ / maDown_)));
		else RSIs.push_back(0.0);

		temp_up_.clear();
        temp_down_.clear();
        pos++;
    }
    alignVectorLength_(RSIs, dataVectorLengthInt_);
}

void  RSI::calculateRSI_VWMA_()
{
    //temporarily variables
    double
            maUp_ = 0.0,
            maDown_ = 0.0;

    std::vector<double>
            temp_price_up_, temp_price_down_, temp_weight_;

    unsigned int pos = 0;

    //getting a vector of slow MAs
    for (int i = dataVectorLengthInt_; i >= nMA_; i--)
    {
        getRollingWindow(data_diffUp_, temp_price_up_, dataVectorLengthInt_, nMA_, pos);
        getRollingWindow(data_diffDown_, temp_price_down_, dataVectorLengthInt_, nMA_, pos);

        getRollingWindow(*weight_, temp_weight_, dataVectorLengthInt_, nMA_, pos);

        //weighting the data
        maUp_ = std::inner_product(temp_price_up_.begin(), temp_price_up_.end(), temp_weight_.begin(), 0.0);
        maDown_ = std::inner_product(temp_price_down_.begin(), temp_price_down_.end(), temp_weight_.begin(), 0.0);

        //writing down RSI on this iteration
		maUp_ = maUp_ / (std::accumulate(temp_weight_.begin(), temp_weight_.end(), 0.0) );
        maDown_ = maDown_/ (std::accumulate(temp_weight_.begin(), temp_weight_.end(), 0.0) );
		if (maDown_ != 0.0) RSIs.push_back( (100 - 100 / (1 + maUp_ / maDown_)));
		else RSIs.push_back(0.0);

        temp_price_up_.clear();
        temp_price_down_.clear();
        temp_weight_.clear();
        pos++;
    }
    alignVectorLength_(RSIs, dataVectorLengthInt_);
}

