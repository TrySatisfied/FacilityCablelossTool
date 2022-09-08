/*#############################################################

                        描述信号的类

##############################################################
 Name: Signal.cpp
 Creat: gary
 Date: 2012/03/2
 Tool: VS2010
 Dscpn: 描述信号的类,当中描述了信号的属性。
***************************************************************/
#include "pch.h"
#include "Signal.h"

#include <vector>

class SignalBase : public Signal
{
public:
	SignalBase():
	  m_power(0),
	  m_power2(0),
	  m_freqHz(0),
	  m_extAtten(0),
	  m_freqGroup(0),
	  m_ant(0),
	  m_ant2(0),
	  m_demod(0),
	  m_bw(0),
	  m_gi(0),
	  m_frameTime(0),
	  m_mcs(0),
	  m_dataLen(0),
	  m_frameGap(0),
	  m_frameCnt(0),
	  m_preamble(0),
	  m_payload(0),
	  m_nss(1),
	  m_isUsed2Port(false)
	  {}
	  
	virtual ~SignalBase(){}

	virtual void SetPow(double dBm){ m_power = dBm; }				//设置功率
	virtual double GetPow() const { return m_power; }				//获取功率

	virtual void SetPow2(double dBm) { m_power2 = dBm; }			//设置功率，仅80+80使用
	virtual double GetPow2() const { return m_power2; }				//获取功率，仅80+80使用

	virtual void SetAntenna(int ant) { m_ant = ant; }				//设置天线
	virtual int GetAntenna() const { return m_ant; }				//获取天线

	virtual void SetAntenna2(int ant) { m_ant2 = ant;}			//设置天线，仅80+80使用
	virtual int GetAntenna2() const { return m_ant2; }			//获取天线，仅80+80使用

	virtual void SetChannel(int channel)							//设置信道
	{
		double freq;

		if (channel <= 14)
		{
			freq = changeChannelToFreq_2_4G(channel);
		}
		else //if (channel < 185)
		{
			freq = changeChannelToFreq_5G(channel);
		}
		//else
		//{
		//	freq = changeChannelToFreq6G(channel);
		//}

		SetFreqHz(freq);
	}

	virtual int GetChannel() const 									//获取信道
	{
		int channel;
		double freq = GetFreqHz();

		if (isFreqIn2_4GGroup(freq))
		{
			channel = changeFreqToChannel_2_4G(freq);
		}
		else if(isFreqIn5GGroup(freq))
		{
			channel = changeFreqToChannel_5G(freq);
		}
		else
		{
			channel = changeFreqToChannel_6G(freq);
		}

		return channel;
	}

	virtual void SetChannel2(int channel)		//设置信道，仅80+80使用
	{
		double freq;

		if (channel <= 14)
		{
			freq = changeChannelToFreq_2_4G(channel);
		}
		else
		{
			freq = changeChannelToFreq_5G(channel);
		}

		SetFreqHz2(freq);
	}
	
	virtual int GetChannel2() const			//获取信道，仅80+80使用
	{
		int channel;
		double freq = GetFreqHz2();

		if (isFreqIn2_4GGroup(freq))
		{
			channel = changeFreqToChannel_2_4G(freq);
		}
		else
		{
			channel = changeFreqToChannel_5G(freq);
		}

		return channel;
	}

	virtual void SetFreqHz(double dHz)								//设置频点
	{
		m_freqHz = dHz;
		if (dHz < 4910e6)	//小于5G视为2.4G组 //Mod for B2006R2 根据MTK添加新的通道
		{
			m_freqGroup = eumFreqGroup2_4G;
		}
		else
		{
			m_freqGroup = eumFreqGroup5G;
		}
	}

	virtual double GetFreqHz() const { return m_freqHz;}				//获取频点

	virtual void SetFreqHz2(double dHz)
	{
		m_freqHz2 = dHz;
		if (dHz < 4910e6)	//小于5G视为2.4G组 //Mod for B2006R2 根据MTK添加新的通道
		{
			m_freqGroup = eumFreqGroup2_4G;
		}
		else
		{
			m_freqGroup = eumFreqGroup5G;
		}
	}

	virtual double GetFreqHz2() const { return m_freqHz2; }			//获取频点，仅80+80使用

	virtual void SetFreqOffsetHz(double dHz) {m_freqOffsetHz = dHz;}	//设置频点偏移
	virtual double GetFreqOffsetHz() const {return m_freqOffsetHz;}		//获取频点偏移

	virtual void SetExtAtten(double dB) { m_extAtten = dB; }		//设置外部增益
	virtual double GetExtAtten() const { return m_extAtten; }		//获取外部增益

	virtual void SetExtAtten2(double dB) { m_extAtten2 = dB; }		//设置外部增益
	virtual double GetExtAtten2() const { return m_extAtten2; }		//获取外部增益

	virtual void SetMcs(int mcs) { m_mcs = mcs; }					//设置MCS
	virtual int GetMcs() const { return m_mcs; }					//获取MCS

	virtual void SetFrameGap(double frameGap) 						//设置帧间隔
	{
		if (frameGap < 0)
		{
			frameGap = 0;
		}

		m_frameGap = frameGap;
	}
	virtual double GetFrameGap() const { return m_frameGap; }		//获取帧间隔

	virtual void SetFrameCnt(int frameCnt) 							//设置帧数
	{
		if (frameCnt < 0)
		{
			frameCnt = 0;
		}

		m_frameCnt = frameCnt;
	}
	virtual int GetFrameCnt() const { return m_frameCnt; }				//获取帧数

	virtual void SetPreamble(int preamble) { m_preamble = preamble; }	//设置长短前导
	virtual int GetPreamble() const { return m_preamble; }				//获取长短前导

	virtual void SetFrameTime(double frameTime) { m_frameTime = frameTime; }	//设置一帧所用时间,us
	virtual double GetFrameTime() const { return m_frameTime; }				//获取一帧采样时间

	virtual void SetDataLen(int dataLen) { m_dataLen = dataLen; }				//设置数据长度
	virtual int GetDataLen() const { return m_dataLen;}						//获取数据长度

	virtual void SetGi(int gi) { m_gi = gi; }						//设置GI模式
	virtual int GetGi() const { return m_gi; }						//获取GI模式

	virtual void SetPayload(int payload) { m_payload = payload; }			//设置Payload
	virtual int GetPayload() const { return m_payload; }					//获取Payload

	virtual double EvluateFramTime() const 			//获取最小采样长度
	{
		double headTime = 220;     // us,帧头占用的时间,不同的标准占用的时间不同

		switch(GetDemod())
		{
		case eumDemod_11ag:
			{
				headTime = 35;
				break;
			}

		case eumDemod_11b:
			{
				headTime = 220;
				break;
			}

		case eumDemod_11n_20M:
		case eumDemod_11n_40M:
			{
				headTime = 45;
				break;
			}

		default:
			headTime = 220;
		}

		double dataTime =  ((GetDataLen() + 4) * 8) / GetDataRate();     // 4 Bytes的CRC
		double frameTime = headTime + dataTime;

		return frameTime;
	}

	virtual double GetDataRate() const { return ChangeMcsToDataRate(m_mcs); }				//获取速率
	virtual int GetFreqGroup() const 			//获取频偏组
	{
		if (isFreqIn2_4GGroup(GetFreqHz()))
		{
			return eumFreqGroup2_4G;
		}
		else if (isFreqIn5GGroup(GetFreqHz()))
		{
			return eumFreqGroup5G;
		}
		else if (isFreqIn6GGroup(GetFreqHz()))
		{
			return eumFreqGroup6G;
		}
		else
		{
			return eumFreqGroupInvalid;
		}
	}
	virtual int GetDemod() const { return m_demod; }					//获取模式
	virtual int GetBw() const { return m_bw; }							//获取带宽

	virtual void SetNss(int nss) { m_nss = nss; }					//设置NSS
	virtual int GetNss() const { return m_nss; }						//获取NSS

	virtual bool IsMcsValid(int mcs) const = 0;			//判断MCS是否合法
	virtual int ChangeDataRateToMcs(double dataRate) const = 0;		//将速率转换成MCS
	virtual double ChangeMcsToDataRate(int mcs) const = 0;				//将MCS转换成速率
	virtual bool IsStandardDataRate(double dataRate) const = 0;		//判断速率是否标准
	virtual bool IsStandardFreq(double freq) const = 0;				//判断频率是否合法 WIFI 按5M间隔进行信道判断
	virtual bool IsChannelValid(int channel) const					//判断channel是否合法
	{
		if (channel <= 14)
		{
			double freq = changeChannelToFreq_2_4G(channel);
			int channelValid = changeFreqToChannel_2_4G(freq);
			return (channelValid == channel);
		}
		else
		{
			double freq = changeChannelToFreq_5G(channel);
			int channelValid = changeFreqToChannel_5G(freq);
			return (channelValid == channel);
		}
	}
	
	virtual void SetUsed2Port(bool isUsed) { m_isUsed2Port = isUsed;}			//设置是否使用2个端口，仅80+80使用
	virtual bool GetUsed2Port() const { return m_isUsed2Port; }				//获取是否使用2个端口，仅80+80使用

protected:
	virtual int changeFreqToChannel_2_4G(double freq) const
	{
		int channel;

		freq -= 2412e6;
		channel = freq / 5e6;
		channel++;

		if (channel > 14)
		{
			channel = 14;
		}
		else if(channel < 1)
		{
			channel = 1;
		}

		return channel;
	}
	virtual double changeChannelToFreq_2_4G(int channel) const
	{
		double step;
		double addtion;

		step = 5e6;    //5MHz
		addtion = 0;

		if (channel < 1)
		{
			channel = 1;
		}
		else if (channel > 13)
		{
			addtion = 12e6; //channel13与channel14的频率步进为12MHz
			channel = 13;
		}

		channel--;

		return 2412e6 + step * channel + addtion;
	}

	virtual int changeFreqToChannel_5G(double freq) const
	{	
		int step = 5;
		int freqMHz = freq / 1e6;
		int channel = 0;

		if (freqMHz < 5000)
		{
			channel = (freqMHz - 4910) / step + 182;
		}
		else
		{
			if ((freqMHz % step) != 0)
			{
				// invalid
				return -1;
			}

			channel = (freqMHz - 5000) / step;
		}

		return channel;
	}
	virtual double changeChannelToFreq_5G(int channel) const
	{
		double freq = 0.0;
		double step = 5e6; // 5MHz

		if (channel >= 182)
		{
			freq = 4910e6 + (channel-182)*step;
		}
		else
		{
			freq = 5000e6 + channel*step;
		}

		return freq;
	}

	virtual int changeFreqToChannel_6G(double freq) const
	{	
		int step = 5;
		int freqMHz = freq / 1e6;
		int channel = 0;

		if ((freqMHz % step) != 0)
		{
			// invalid
			return -1;
		}

		//Mod for B05T10,6G频点与channel值对改为与最新meter的一致(channel号从1开始)
		channel = (freqMHz - 5950) / step;
		////channel = (freqMHz - 5925) / step + 185;

		return channel;
	}
	virtual double changeChannelToFreq6G(int channel) const
	{
		double freq = 0.0;
		double step = 5e6; // 5MHz

		//Mod for B05T10,6G频点与channel值对改为与最新meter的一致(channel好从1开始)				   
		freq = 5950e6 + channel*step;
		////freq = 5925e6 + (channel - 185)*step;

		return freq;
	}

	virtual bool isFreqIn2_4GGroup(double freq) const
	{
		if ((freq < 2412e6) || (freq > 2484e6))
		{
			return false;
		}

		return true;
	}
	virtual bool isFreqIn5GGroup(double freq) const
	{
		if ((freq < 4910e6) || (freq > 5905e6))
		{
			return false;
		}

		return true;
	}


	virtual bool isFreqIn6GGroup(double freq) const
	{
		////if ((freq < 5925e6) || (freq > 7125e6))
		if ((freq < 5955e6) || (freq > 7125e6))
		{
			return false;
		}

		return true;
	}
	//返回信号持续时间,单位us
	virtual double getSigTime_us(int frameCount) const
	{
		double sigTime;

		sigTime = GetFrameTime();
		sigTime += GetFrameGap() / 5;
		sigTime += 50;                    // 再加50us的保护间隔
		sigTime *= frameCount;

		return sigTime;
	}

protected:
	double m_power;			//dBm
	double m_power2;		//dBm，仅80+80使用
	double m_freqHz;		//Hz
	double m_freqHz2;		//Hz，仅80+80使用
	double m_freqOffsetHz;	//Hz
	double m_extAtten;		//dB,与Pow的关系:Pow表示没有'外部衰减'的理想功率,实际功率为Pow-ExtAtten
	double m_extAtten2;		//dB，仅80+80使用
	int m_freqGroup;		//eumFreqGroupxxG
	int m_ant;
	int m_ant2;				//仅80+80使用

	int m_demod;			//eumDemod枚举
	int m_bw;				//带宽,MHz
	int m_gi;				//11n专用
	double m_frameTime;		//一帧所用时间,us
	int  m_mcs;				//统一使用Mcs
	int m_dataLen;			//内容长度

	double m_frameGap;		//帧间隙,us
	int m_frameCnt;			//帧数
	int m_preamble;         //长短前导 1：Long， 0:short
	int m_payload;			//BT
	int m_nss;				//11ac nss流

	bool m_isUsed2Port;		//是否使用2个端口，仅80+80使用
};

// wifi base
class SignalWifi : public SignalBase
{
public:
	SignalWifi()
	{
		
	}
	virtual ~SignalWifi(){}

	virtual void SetGi(int gi) 
	{ 
		m_gi = gi;

		if (gi == eumGiLong || gi == eumGiLTF2)
		{
			m_dataRate = m_dataRateLongGi;
		}
		else 
		{
			m_dataRate = m_dataRateShortGi;
		}
	}

	virtual bool IsMcsValid(int mcs) const
	{
		return (0 <= mcs) && (mcs < m_dataRate.size());
	}

	virtual bool IsStandardFreq(double freq) const
	{
		int channel;
		double standardFreq;
		if (isFreqIn5GGroup(freq))
		{
			channel = changeFreqToChannel_5G(freq);
			standardFreq = changeChannelToFreq_5G(channel);
			return (freq == standardFreq);
		}
		else if (isFreqIn2_4GGroup(freq))
		{
			channel = changeFreqToChannel_2_4G(freq);
			standardFreq = changeChannelToFreq_2_4G(channel);
			return (freq == standardFreq);
		}
		else if(isFreqIn6GGroup(freq))
		{
			channel = changeFreqToChannel_6G(freq);
			standardFreq = freq;
			return (freq == standardFreq);
		}

		return false;
	}

	virtual bool IsStandardDataRate(double dataRate) const
	{
		for (auto iter = m_dataRate.cbegin(); iter != m_dataRate.cend(); iter++)
		{
			if (dataRate == *iter)
			{
				return true;
			}
		}

		return false;
	}

	virtual int ChangeDataRateToMcs(double dataRate) const
	{
		int mcs = -1;

		for (int i = 0; i < m_dataRate.size(); i++)
		{
			if (dataRate <= m_dataRate[i])
			{
				mcs = i;
				break;
			}
		}

		if (eumDemod_11ac_20M == m_demod)
		{
			mcs %= 9;
		}
		else if (eumDemod_11ac_20M < m_demod && m_demod <= eumDemod_11ac_160M)
		{
			mcs %= 10;
		}
		else if (eumDemod_11ax_20M < m_demod && m_demod <= eumDemod_11ax_160M)
		{
			mcs %= 12;
		}

		return mcs;
	}

	virtual double ChangeMcsToDataRate(int mcs) const
	{
		if (0 > mcs || mcs >= m_dataRate.size())
		{
			return -999.0;
		}

		if (eumDemod_11ac_20M == m_demod)
		{
			mcs += 9 * (m_nss-1);
		}
		else if (eumDemod_11ac_20M < m_demod && m_demod <= eumDemod_11ac_80M_80M)
		{
			mcs += 10 * (m_nss-1);
		}
		else if (eumDemod_11ax_20M <= m_demod && m_demod <= eumDemod_11ax_80_80M)
		{
			mcs += 12 * (m_nss-1);
		}

		if (mcs >= m_dataRate.size())
		{
			return -999.0;
		}

		return m_dataRate[mcs];
	}

protected:
	std::vector<double> m_dataRate;
	std::vector<double> m_dataRateLongGi;
	std::vector<double> m_dataRateShortGi;
};

//11ag信号
class Signal11ag : public SignalWifi
{
public:
	Signal11ag()
	{
		m_demod = eumDemod_11ag;
		m_bw = eumBw_20M;
		m_gi = eumGiLong;

		double dataRate[] = {6, 9, 12, 18, 24, 36, 48, 54};
		m_dataRateLongGi.assign(std::begin(dataRate), std::end(dataRate));
		m_dataRate = m_dataRateShortGi = m_dataRateLongGi;
	}
	~Signal11ag(){}
};

//11b信号
class Signal11b
	:public SignalWifi
{
public:
	Signal11b()
	{
		m_demod = eumDemod_11b;
		m_bw = eumBw_20M;
		m_gi = eumGiLong;

		double dataRate[] = {1, 2, 5.5, 11};
		m_dataRateLongGi.assign(std::begin(dataRate), std::end(dataRate));
		m_dataRate = m_dataRateShortGi = m_dataRateLongGi;
	}
	~Signal11b() {}

	bool IsStandardFreq(double freq) const
	{
		if (isFreqIn5GGroup(freq))
		{
			return false;
		}

		int channel = changeFreqToChannel_2_4G(freq);
		double standardFreq = changeChannelToFreq_2_4G(channel);
		return (freq == standardFreq);
	}
};

//11n20m信号
class Signal11n20m
	:public SignalWifi
{
public:
	Signal11n20m()
	{
		m_demod = eumDemod_11n_20M;
		m_bw = eumBw_20M;
		m_gi = eumGiLong;

		double dataRateLong[] = 
		{
			6.5,  13.0, 19.5, 26.0,  39.0,  52.0,  58.5,  65.0,		// 1X1
			13.0, 26.0, 39.0, 52.0,  78.0,  104.0, 117.0, 130.0,	// 2X2
			19.5, 39.0, 58.5, 78.0,  117.0, 156.0, 175.5, 195.0,	// 3X3
			26.0, 52.0, 78.0, 104.0, 156.0, 208.0, 234.0, 260.0		// 4X4
		};
		m_dataRateLongGi.assign(std::begin(dataRateLong), std::end(dataRateLong));
		m_dataRate = m_dataRateLongGi;

		double dataRateShort[] = 
		{
			7.2,  14.2, 21.7, 28.9,  43.3,  57.8,  65.0,  72.2, 	// 1X1
			14.4, 28.9, 43.3, 57.8,  86.7,  115.6, 130.0, 144.4,	// 2X2
			21.7, 43.3, 65.0, 86.7,  130.0, 173.3, 195.0, 216.7,	// 3X3
			28.9, 57.8, 86.7, 115.6, 173.3, 231.1, 260.0, 188.9		// 4X4
		};
		m_dataRateShortGi.assign(std::begin(dataRateShort), std::end(dataRateShort));
	}
	~Signal11n20m(){}
};

//11n40m信号
class Signal11n40m
	:public SignalWifi
{
public:
	Signal11n40m()
	{
		m_demod = eumDemod_11n_40M;
		m_bw = eumBw_40M;
		m_gi = eumGiLong;

		double dataRateLong[] = 
		{
			13.5, 27.0,  40.5,  54.0,  81.0,  108.0, 121.5, 135.0,	// 1X1
			27.0, 54.0,  81.0,  108.0, 162.0, 216.0, 243.0, 270.0,	// 2X2
			40.5, 81.0,  121.5, 162.0, 243.0, 324.0, 364.5, 405.0,	// 3X3
			54.0, 108.0, 162.0, 216.0, 324.0, 432.0, 486.0, 540.0	// 4X4
		};
		m_dataRateLongGi.assign(std::begin(dataRateLong), std::end(dataRateLong));
		m_dataRate = m_dataRateLongGi;

		double dataRateShort[] = 
		{
			15.0, 30.0,  45.0,  60.0,  90.0,  120.0, 135.0, 150.0,	// 1X1
			30.0, 60.0,  90.0,  120.0, 180.0, 240.0, 270.0, 300.0,	// 2X2
			45.0, 90.0,  135.0, 180.0, 270.0, 360.0, 405.0, 450.0,	// 3X3
			60.0, 120.0, 180.0, 240.0, 360.0, 480.0, 540.0, 600.0	// 4X4
		};
		m_dataRateShortGi.assign(std::begin(dataRateShort), std::end(dataRateShort));
	}
};



//11ac20m信号
class Signal11ac20m
	:public SignalWifi
{
public:
	Signal11ac20m()
	{
		m_demod = eumDemod_11ac_20M;
		m_bw = eumBw_20M;
		m_gi = eumGiLong;

		double dataRateLong[] = 
		{
			6.5,  13.0, 19.5, 26.0,  39.0,  52.0,  58.5,  65.0,	 78.0,  86.5,  97.5,  108.3, 117.0, 130,	// 1X1
			13.0, 26.0, 39.0, 52.0,  78.0,  104.0, 117.0, 130.0, 156.0, 173.0, 195.0, 216.6, 234.0, 260,// 2X2
			19.5, 39.0, 58.5, 78.0,  117.0, 156.0, 175.5, 195.0, 234.0,	259.5, 292.5, 324.9, 351.0, 390,// 3X3
			26.0, 52.0, 78.0, 104.0, 156.0, 208.0, 234.0, 260.0, 312.0,	346.0, 390.0, 433.2, 468.0, 520,// 4X4
		};
		m_dataRateLongGi.assign(std::begin(dataRateLong), std::end(dataRateLong));
		m_dataRate = m_dataRateLongGi;

		double dataRateShort[] = 
		{
			7.2,  14.4, 21.7, 28.9,  43.3,  57.8,  65.0,  72.2,  86.7,	// 1X1
			14.4, 28.8, 43.4, 57.8,  86.6,  115.6, 130.0, 144.4, 173.4,	// 2X2
			21.6, 43.2, 65.1, 86.7,  129.9, 173.4, 195.0, 216.6, 260.1,	// 3X3
			28.8, 57.6, 86.8, 115.6, 173.2, 231.2, 260.0, 288.8, 346.8	// 4X4
		};
		m_dataRateShortGi.assign(std::begin(dataRateShort), std::end(dataRateShort));
	}
};

//11ac40m信号
class Signal11ac40m
	:public SignalWifi
{
public:
	Signal11ac40m()
	{
		m_demod = eumDemod_11ac_40M;
		m_bw = eumBw_40M;
		m_gi = eumGiLong;

		double dataRateLong[] = 
		{
			13.5, 27.0,  40.5,  54.0,  81.0,  108.0, 121.5, 135.0, 162.0, 180.0, 202.5,	225.0, 243.0, 270,  // 1X1
			27.0, 54.0,  81.0,  108.0, 162.0, 216.0, 243.0, 270.0, 324.0, 360.0, 405.0, 450.0, 486.0, 540, 	// 2X2
			40.5, 81.0,  121.5, 162.0, 243.0, 324.0, 364.5, 405.0, 486.0, 540.0, 607.5, 675.0, 729.0, 810,// 3X3
			54.0, 108.0, 162.0, 216.0, 324.0, 432.0, 486.0, 540.0, 648.0, 720.0, 810.0, 900.0, 972.0, 1080, 		// 4X4
		};
		m_dataRateLongGi.assign(std::begin(dataRateLong), std::end(dataRateLong));
		m_dataRate = m_dataRateLongGi;

		double dataRateShort[] = 
		{
			15.0, 30.0,  45.0,  60.0,  90.0,  120.0, 135.0, 150.0, 180.0, 200.0,	// 1X1
			30.0, 60.0,  90.0,  120.0, 180.0, 240.0, 270.0, 300.0, 360.0, 400.0,	// 2X2
			45.0, 90.0,  135.0, 180.0, 270.0, 360.0, 405.0, 450.0, 540.0, 600.0,	// 3X3
			60.0, 120.0, 180.0, 240.0, 360.0, 480.0, 540.0, 600.0, 720.0, 800.0		// 4X4
		};
		m_dataRateShortGi.assign(std::begin(dataRateShort), std::end(dataRateShort));
	}
};

//11ac80m信号
class Signal11ac80m
	:public SignalWifi
{
public:
	Signal11ac80m()
	{
		m_demod = eumDemod_11ac_80M;
		m_bw = eumBw_80M;
		m_gi = eumGiLong;

		double dataRateLong[] = 
		{
			29.3,  58.5,  87.8,  117.0, 175.5, 234.0, 263.3,  292.5,  351.0,  390.0,  438.8,  487.5, 526.5,  585.0,	// 1X1
			58.6,  117.0, 175.6, 234.0, 351.0, 468.0, 526.6,  585.0,  702.0,  780.0,  877.6,  975.0, 1053.0, 1170.0,     // 2X2
			87.9,  175.5, 263.4, 351.0, 526.5, 702.0, -999.0, 877.5,  1053.0, 1170.0, 1316.4, 1462.5,1579.5, 1755.0,  	// 3X3 不支持MCS6
			117.2, 234.0, 351.2, 468.0, 702.0, 936.0, 1053.2, 1170.0, 1404.0, 1560.0, 1755.2, 1950.0,2106.0, 2340.0,  // 4X4
		};
		m_dataRateLongGi.assign(std::begin(dataRateLong), std::end(dataRateLong));
		m_dataRate = m_dataRateLongGi;

		double dataRateShort[] = 
		{
			32.5,  65.0,  97.5,  130.0, 195.0, 260.0,  292.5,  325.0,  390.0,  433.3,	// 1X1
			65.0,  130.0, 195.0, 260.0, 390.0, 520.0,  585.0,  650.0,  780.0,  866.6,	// 2X2
			97.5,  195.0, 292.5, 390.0, 585.0, 780.0,  -999,   975.0,  1170.0, 1299.9,	// 3X3
			130.0, 260.0, 390.0, 520.0, 780.0, 1040.0, 1170.0, 1300.0, 1560.0, 1733.2		// 4X4
		};
		m_dataRateShortGi.assign(std::begin(dataRateShort), std::end(dataRateShort));
	}
};


//11ac80_80m信号
class Signal11ac80_80m
	:public SignalWifi
{
public:
	Signal11ac80_80m()
	{
		m_demod = eumDemod_11ac_80M_80M;
		m_bw = eumBw_80_80M;
		m_gi = eumGiLong;

		double dataRateLong[] = 
		{
			58.6,  117.0, 175.6, 234.0, 351.0, 468.0, 526.6,  585.0,  702.0,  780.0,	// 相当于80M的2X2，待确认
			87.9,  175.5, 263.4, 351.0, 526.5, 702.0, -999.0, 877.5,  1053.0, 1170.0,	// 3X3 不支持MCS6
			117.2, 234.0, 351.2, 468.0, 702.0, 936.0, 1053.2, 1170.0, 1404.0, 1560.0	// 4X4
		};
		m_dataRateLongGi.assign(std::begin(dataRateLong), std::end(dataRateLong));
		m_dataRate = m_dataRateLongGi;

		double dataRateShort[] = 
		{
			65.0,  130.0, 195.0, 260.0, 390.0, 520.0,  585.0,  650.0,  780.0,  866.6,	// 相当于80M的2X2，待确认
			97.5,  195.0, 292.5, 390.0, 585.0, 780.0,  -999,   975.0,  1170.0, 1299.9,	// 3X3
			130.0, 260.0, 390.0, 520.0, 780.0, 1040.0, 1170.0, 1300.0, 1560.0, 1733.2		// 4X4
		};
		m_dataRateShortGi.assign(std::begin(dataRateShort), std::end(dataRateShort));
	}
};

//11ac160m信号
class Signal11ac160m
	:public SignalWifi
{
public:
	Signal11ac160m()
	{
		m_demod = eumDemod_11ac_160M;
		m_bw = eumBw_160M;
		m_gi = eumGiLong;

		double dataRateLong[] = 
		{
			58.6,  117.0, 175.6, 234.0, 351.0, 468.0, 526.6,  585.0,  702.0,  780.0, 877.5,  975.0, 1053.0, 1170,	// 相当于80M的2X2，待确认
			87.9,  175.5, 263.4, 351.0, 526.5, 702.0, -999.0, 877.5,  1053.0, 1170.0,1316.4, 1462.5,1579.5, 1755.0,	// 3X3 不支持MCS6
			117.2, 234.0, 351.2, 468.0, 702.0, 936.0, 1053.2, 1170.0, 1404.0, 1560.0,1755.2, 1950.0,2106.0, 2340.0	// 4X4
		};
		m_dataRateLongGi.assign(std::begin(dataRateLong), std::end(dataRateLong));
		m_dataRate = m_dataRateLongGi;

		double dataRateShort[] = 
		{
			65.0,  130.0, 195.0, 260.0, 390.0, 520.0,  585.0,  650.0,  780.0,  866.6,	// 相当于80M的2X2，待确认
			97.5,  195.0, 292.5, 390.0, 585.0, 780.0,  -999,   975.0,  1170.0, 1299.9,	// 3X3
			130.0, 260.0, 390.0, 520.0, 780.0, 1040.0, 1170.0, 1300.0, 1560.0, 1733.2		// 4X4
		};
		m_dataRateShortGi.assign(std::begin(dataRateShort), std::end(dataRateShort));
	}
};

//11ax20m信号
class Signal11ax20m
	:public SignalWifi
{
public:
	Signal11ax20m()
	{
		m_demod = eumDemod_11ax_20M;
		m_bw = eumBw_20M;
		m_gi = eumGiLTF0;

		double dataRateLong[] = 
		{
			8.0,  16.0, 24.0, 33.0,  49.0,  65.0,  73.0,  81.0,	 98.0,  108.0, 122.0, 135.0, 154.0, 172.0, // 1X1
			//13.0, 26.0, 39.0, 52.0,  78.0,  104.0, 117.0, 130.0, 156.0,	// 2X2
			//19.5, 39.0, 58.5, 78.0,  117.0, 156.0, 175.5, 195.0, 234.0,	// 3X3
			//26.0, 52.0, 78.0, 104.0, 156.0, 208.0, 234.0, 260.0, 312.0	// 4X4
		};
		m_dataRateLongGi.assign(std::begin(dataRateLong), std::end(dataRateLong));
		//m_dataRate = m_dataRateLongGi;

		double dataRateShort[] = 
		{
			8.6,  17.2, 25.8, 34.4,  51.6,  68.8,  77.4,  86.0,  103.0,	 115.0, 129.0, 143.5, 154.9, 172.1,  // 1X1
			//14.4, 28.8, 43.4, 57.8,  86.6,  115.6, 130.0, 144.4, 173.4,	// 2X2
			//21.6, 43.2, 65.1, 86.7,  129.9, 173.4, 195.0, 216.6, 260.1,	// 3X3
			//28.8, 57.6, 86.8, 115.6, 173.2, 231.2, 260.0, 288.8, 346.8	// 4X4
		};
		m_dataRateShortGi.assign(std::begin(dataRateShort), std::end(dataRateShort));
		m_dataRate = m_dataRateShortGi;
	}
};

//11ax40m信号
class Signal11ax40m
	:public SignalWifi
{
public:
	Signal11ax40m()
	{
		m_demod = eumDemod_11ax_40M;
		m_bw = eumBw_40M;
		m_gi = eumGiLTF0;

		double dataRateLong[] = 
		{
			16.0,  33.0,  49.0,  65.0,  98.0,  130.0, 146.0, 163.0, 195.0, 217.0, 244.0, 271.0, 309.0, 344.0	// 1X1
			//27.0, 54.0,  81.0,  108.0, 162.0, 216.0, 243.0, 270.0, 324.0, 360.0,	// 2X2
			//40.5, 81.0,  121.5, 162.0, 243.0, 324.0, 364.5, 405.0, 486.0, 540.0,	// 3X3
			//54.0, 108.0, 162.0, 216.0, 324.0, 432.0, 486.0, 540.0, 648.0, 720.0		// 4X4
		};
		m_dataRateLongGi.assign(std::begin(dataRateLong), std::end(dataRateLong));
		//m_dataRate = m_dataRateLongGi;

		double dataRateShort[] = 
		{
			17.2,  34.4,  51.6,  68.8,  103.2, 137.6, 154.9, 172.1, 206.5, 229.4, 258.1, 286.8, 309.7, 344.1,	// 1X1
			//30.0, 60.0,  90.0,  120.0, 180.0, 240.0, 270.0, 300.0, 360.0, 400.0,	// 2X2
			//45.0, 90.0,  135.0, 180.0, 270.0, 360.0, 405.0, 450.0, 540.0, 600.0,	// 3X3
			//60.0, 120.0, 180.0, 240.0, 360.0, 480.0, 540.0, 600.0, 720.0, 800.0		// 4X4
		};
		m_dataRateShortGi.assign(std::begin(dataRateShort), std::end(dataRateShort));
		m_dataRate = m_dataRateShortGi;
	}
};

//11ax80m信号
class Signal11ax80m
	:public SignalWifi
{
public:
	Signal11ax80m()
	{
		m_demod = eumDemod_11ax_80M;
		m_bw = eumBw_80M;
		m_gi = eumGiLTF0;

		double dataRateLong[] = 
		{
			34.0, 68.0, 102.0, 136.0, 204.0, 272.0, 306.0, 340.0, 408.0, 453.0, 510.0, 567.0, 648.5, 720.6	// 1X1
			//58.6,  117.0, 175.6, 234.0, 351.0, 468.0, 526.6,  585.0,  702.0,  780.0,	// 2X2
			//87.9,  175.5, 263.4, 351.0, 526.5, 702.0, -999.0, 877.5,  1053.0, 1170.0,	// 3X3 不支持MCS6
			//117.2, 234.0, 351.2, 468.0, 702.0, 936.0, 1053.2, 1170.0, 1404.0, 1560.0	// 4X4
		};
		m_dataRateLongGi.assign(std::begin(dataRateLong), std::end(dataRateLong));
		//m_dataRate = m_dataRateLongGi;

		double dataRateShort[] = 
		{
			36.0, 72.1, 108.1, 144.1, 216.2, 288.2, 324.4, 360.3, 432.4, 480.4, 540.4, 600.5, 648.5, 720.6	// 1X1
			//65.0,  130.0, 195.0, 260.0, 390.0, 520.0,  585.0,  650.0,  780.0,  866.6,	// 2X2
			//97.5,  195.0, 292.5, 390.0, 585.0, 780.0,  -999,   975.0,  1170.0, 1299.9,	// 3X3
			//130.0, 260.0, 390.0, 520.0, 780.0, 1040.0, 1170.0, 1300.0, 1560.0, 1733.2		// 4X4
		};
		m_dataRateShortGi.assign(std::begin(dataRateShort), std::end(dataRateShort));
		m_dataRate = m_dataRateShortGi;
	}
};


//11ax80_80m信号
class Signal11ax80_80m
	:public SignalWifi
{
public:
	Signal11ax80_80m()
	{
		m_demod = eumDemod_11ax_80_80M;
		m_bw = eumBw_80_80M;
		m_gi = eumGiLTF0;

		double dataRateLong[] = 
		{
			34.0,  136.0,  204.0, 272.0, 408.0, 544.0, 612.0,  680.0,  816.0,  906.0, 1020.0, 1134.0, 1297.0, 1441.0	// 相当于80M的2X2，待确认
			//87.9,  175.5, 263.4, 351.0, 526.5, 702.0, -999.0, 877.5,  1053.0, 1170.0,	// 3X3 不支持MCS6
			//117.2, 234.0, 351.2, 468.0, 702.0, 936.0, 1053.2, 1170.0, 1404.0, 1560.0	// 4X4
		};
		m_dataRateLongGi.assign(std::begin(dataRateLong), std::end(dataRateLong));
		//m_dataRate = m_dataRateLongGi;

		double dataRateShort[] = 
		{
			36.0,  144.0,  216.0,  288.0, 432.0, 576.0,  648.0,  720.0,  864.0,  960.0, 1080.0, 1200.0, 1297.0, 1441.0	// 相当于80M的2X2，待确认
			//97.5,  195.0, 292.5, 390.0, 585.0, 780.0,  -999,   975.0,  1170.0, 1299.9,	// 3X3
			//130.0, 260.0, 390.0, 520.0, 780.0, 1040.0, 1170.0, 1300.0, 1560.0, 1733.2		// 4X4
		};
		m_dataRateShortGi.assign(std::begin(dataRateShort), std::end(dataRateShort));
		m_dataRate = m_dataRateShortGi;
	}
};

//11ax160m信号
class Signal11ax160m
	:public SignalWifi
{
public:
	Signal11ax160m()
	{
		m_demod = eumDemod_11ax_160M;
		m_bw = eumBw_160M;
		m_gi = eumGiLTF0;

		double dataRateLong[] = 
		{
			68.0,  136.0,  204.0, 272.0, 408.0, 544.0, 613.0,  681.0,  817.0,  907.0, 1021.0, 1134.0, 1297.1, 1441.2
			//87.9,  175.5, 263.4, 351.0, 526.5, 702.0, -999.0, 877.5,  1053.0, 1170.0,	// 3X3 不支持MCS6
			//117.2, 234.0, 351.2, 468.0, 702.0, 936.0, 1053.2, 1170.0, 1404.0, 1560.0	// 4X4
		};
		m_dataRateLongGi.assign(std::begin(dataRateLong), std::end(dataRateLong));
		//m_dataRate = m_dataRateLongGi;

		double dataRateShort[] = 
		{
			72.1,  144.1,  216.2,  282.2, 432.4, 576.5,  648.5,  720.6,  864.7,  960.7, 1080.9, 1201.0, 1297.1, 1441.2
			//97.5,  195.0, 292.5, 390.0, 585.0, 780.0,  -999,   975.0,  1170.0, 1299.9,	// 3X3
			//130.0, 260.0, 390.0, 520.0, 780.0, 1040.0, 1170.0, 1300.0, 1560.0, 1733.2		// 4X4
		};
		m_dataRateShortGi.assign(std::begin(dataRateShort), std::end(dataRateShort));
		m_dataRate = m_dataRateShortGi;
	}
};


//蓝牙信号
class SignalBT
	:public SignalBase
{
public:
	SignalBT()
	{
		SetFreqHz(2402e6);
		m_demod = eumDemod_BT;
	}

	virtual bool IsMcsValid(int mcs) const
	{
		return 0 <= mcs && mcs < 4;
	}

	virtual int ChangeDataRateToMcs(double dataRate) const
	{
		double darMbps[] = {1, 2, 3};
		int nMbpsCont;
		int i;
		bool bRslt;

		nMbpsCont = sizeof(darMbps) / sizeof(darMbps[0]);
		bRslt = false;

		for (i = 0; i < nMbpsCont; i++)
		{
			if (dataRate <= darMbps[i])
			{
				bRslt = true;
				break;
			}
		}

		if (bRslt == false)
		{
			i--;
		}

		return i;
	}

	virtual double ChangeMcsToDataRate(int mcs) const
	{
		double darMbps[] = {1, 1, 1, 2, 2, 2, 3, 3, 3, 1};
		int nMbpsCont;

		nMbpsCont = sizeof(darMbps) / sizeof(darMbps[0]);

		if (mcs >= nMbpsCont)
		{
			mcs = nMbpsCont - 1;
		}

		return darMbps[mcs];
	}

	virtual bool IsStandardDataRate(double dataRate) const
	{
		double darMbps[] = {1, 2, 3};

		for (int i = 0; i < (sizeof(darMbps) / sizeof(darMbps[0])); i++)
		{
			if (dataRate == darMbps[i])
			{
				return true;
			}
		}

		return false;
	}

	virtual bool IsStandardFreq(double freq) const
	{
		int iChannel = changeFreqToChannel_2_4G(freq);
		double dStandardFreq = changeChannelToFreq_2_4G(iChannel);
		return (freq == dStandardFreq);
	}

	virtual double EvluateFramTime() const
	{
		double dHeadTime = 126;     //us,蓝牙的head固定取值
		double dDataTime =  (GetDataLen() * 8) / GetDataRate();
		double dFrameTime = dHeadTime + dDataTime;

		return dFrameTime;
	}

	virtual bool IsChannelValid(int channel) const
	{
		int	maxChannel = m_mcs == enumBT_LE ? 39 : 78;
		if (channel <= maxChannel)
		{
			double dFreq = changeChannelToFreq_2_4G(channel);
			int iChannelValid = changeFreqToChannel_2_4G(dFreq);
			return (iChannelValid == channel);
		}
		return false;
	}

	virtual void SetChannel(int channel)
	{
		double dFreq;
		dFreq = changeChannelToFreq_2_4G(channel);
		SetFreqHz(dFreq);
		return;
	}

	virtual int GetChannel() const
	{
		int iChannel;
		double dFreq = GetFreqHz();
		iChannel = changeFreqToChannel_2_4G(dFreq);
		return iChannel;
	}

protected:
	bool isFreqIn2_4GGroup(double freq) const
	{
		if ((freq < 2402e6) || (freq > 2480e6))
		{
			return false;
		}

		return true;
	}

	int changeFreqToChannel_2_4G(double freq) const
	{
		int channel = 0;
		double step;// = m_mcs == enumBT_LE ? 2e6 : 1e6;
		int	maxChannel;// = m_mcs == enumBT_LE ? 39 : 78;

		if(m_mcs == enumBT_LE1M || m_mcs == enumBT_LE2M || m_mcs == enumBT_LE_125K || m_mcs == enumBT_LE_500K)
		{
			step = 2e6;
			maxChannel = 39;
		}
		else
		{
			step = 1e6;
			maxChannel = 78;
		}

		freq -= 2402e6;
		channel = freq / step;

		if (channel > maxChannel)
		{
			channel = maxChannel;
		}
		else if(channel < 0)
		{
			channel = 0;
		}

		return channel;
	}

	double changeChannelToFreq_2_4G(int channel) const
	{
		double step;// = m_mcs == enumBT_LE ? 2e6 : 1e6;
		int	maxChannel;// = m_mcs == enumBT_LE ? 39 : 78;

		if(m_mcs == enumBT_LE1M || m_mcs == enumBT_LE2M || m_mcs == enumBT_LE_125K || m_mcs == enumBT_LE_500K)
		{
			step = 2e6;
			maxChannel = 39;
		}
		else
		{
			step = 1e6;
			maxChannel = 78;
		}

		if (channel < 0)
		{
			channel = 0;
		}
		else if (channel > maxChannel)
		{
			channel = maxChannel;
		}

		return 2402e6 + step * channel;
	}
};

//Zigbee 信号
class SignalZigbee
	:public SignalBase
{
public:
	SignalZigbee()
	{
		SetFreqHz(2405e6);
		m_demod = eumDemod_Zigbee;
		m_bw = enumZigbee;
		m_gi = eumGiLong;
	}

	virtual bool IsMcsValid(int mcs) const
	{
		return true;
	}

	virtual int ChangeDataRateToMcs(double dataRate) const
	{
		// 暂不实现
		return -1;
	}

	virtual double ChangeMcsToDataRate(int mcs) const
	{
		return 0;
	}

	virtual bool IsStandardDataRate(double dataRate) const
	{
		// 暂不实现
		return false;
	}

	virtual bool IsStandardFreq(double freq) const
	{
		int iChannel = changeFreqToChannel_2_4G(freq);
		double dStandardFreq = changeChannelToFreq_2_4G(iChannel);
		return (freq == dStandardFreq);
	}

	virtual bool IsChannelValid(int channel) const
	{
		int	maxChannel = 26;
		if (channel <= maxChannel)
		{
			double dFreq = changeChannelToFreq_2_4G(channel);
			int iChannelValid = changeFreqToChannel_2_4G(dFreq);
			return (iChannelValid == channel);
		}
		return false;
	}

	virtual void SetChannel(int channel)
	{
		double dFreq;
		dFreq = changeChannelToFreq_2_4G(channel);
		SetFreqHz(dFreq);
		return;
	}

	virtual int GetChannel() const
	{
		int iChannel;
		double dFreq = GetFreqHz();
		iChannel = changeFreqToChannel_2_4G(dFreq);
		return iChannel;
	}

protected:
	bool isFreqIn2_4GGroup(double freq) const
	{
		if ((freq < 2405e6) || (freq > 2480e6))
		{
			return false;
		}

		return true;
	}

	int changeFreqToChannel_2_4G(double freq) const
	{
		int iChannel = 0;

		freq -= 2405e6;
		iChannel = (freq / (5*1e6))+11;


		if (iChannel > 26)
		{
			iChannel = 26;
		}
		else if(iChannel < 11)
		{
			iChannel = 11;
		}

		return iChannel;
	}

	double changeChannelToFreq_2_4G(int channel) const
	{
		double dStep = 1e6;    //1MHz

		if (channel < 11)
		{
			channel = 11;
		}
		else if (channel > 26)
		{
			channel = 26;
		}

		return 2405e6 + dStep * (channel - 11)*5;
	}
};

//GPS 信号
class SignalGPS
	:public SignalBase
{
public:
	SignalGPS()
	{
		SetFreqHz(1575e6);
		m_demod = eumDemod_GPS;
		m_freqGroup = eumFreqGroup2_4G;
	}

	virtual bool IsMcsValid(int mcs) const
	{
		return true;
	}

	virtual int ChangeDataRateToMcs(double dataRate) const
	{
		return 0;
	}

	virtual double ChangeMcsToDataRate(int mcs) const
	{
		return 0;
	}

	virtual bool IsStandardDataRate(double dataRate) const
	{
		return false;
	}

	virtual bool IsStandardFreq(double freq) const
	{
		//if (1575e6 != freq)
		//{
		//	return false;
		//}
		return true;
	}

	virtual bool IsChannelValid(int channel) const
	{
		//if (1 != channel)
		//{
		//	return false;
		//}
		return true;
	}

	virtual void SetChannel(int channel)
	{
		double dFreq;
		dFreq = changeChannelToFreq(channel);
		SetFreqHz(dFreq);
		return;
	}

	virtual int GetChannel() const
	{
		int iChannel;
		double dFreq = GetFreqHz();
		iChannel = changeFreqToChannel(dFreq);
		return iChannel;
	}

	virtual bool isFreqIn2_4GGroup(double freq) const
	{
		return true;
	}

protected:

	int changeFreqToChannel(double freq) const
	{
		if (freq == 1575e6)
		{
			return 1;
		}

		return freq/1e6;
	}

	double changeChannelToFreq(int channel) const
	{
		if (channel == 1)
		{
			return 1575e6;
		}
		else
		{
			return channel;
		}
	}
};

//BDS 信号
class SignalBDS
	:public SignalBase
{
public:
	SignalBDS()
	{
		SetFreqHz(1561e6);
		m_demod = eumDemod_BDS;
		m_freqGroup = eumFreqGroup2_4G;
	}

	virtual bool IsMcsValid(int mcs) const
	{
		return true;
	}

	virtual int ChangeDataRateToMcs(double dataRate) const
	{
		return 0;
	}

	virtual double ChangeMcsToDataRate(int mcs) const
	{
		return 0;
	}

	virtual bool IsStandardDataRate(double dataRate) const
	{
		return false;
	}

	virtual bool IsStandardFreq(double freq) const
	{
		//if (1561e6 != freq)
		//{
		//	return false;
		//}
		return true;
	}

	virtual bool IsChannelValid(int channel) const
	{
		//if (1 != channel)
		//{
		//	return false;
		//}
		return true;
	}

	virtual void SetChannel(int channel)
	{
		double dFreq;
		dFreq = changeChannelToFreq(channel);
		SetFreqHz(dFreq);
		return;
	}

	virtual int GetChannel() const
	{
		int iChannel;
		double dFreq = GetFreqHz();
		iChannel = changeFreqToChannel(dFreq);
		return iChannel;
	}

	virtual bool isFreqIn2_4GGroup(double freq) const
	{
		return true;
	}

protected:

	int changeFreqToChannel(double freq) const
	{
		if (freq == 1561e6)
		{
			return 1;
		}

		return freq/1e6;
	}

	double changeChannelToFreq(int channel) const
	{
		if (channel == 1)
		{
			return 1561e6;
		}
		else
		{
			return channel;
		}
	}
};

extern "C" SIGNAL_Decl Signal * CreateSignal( int demode )
{
	Signal *tmpSignal = nullptr;

	switch (demode)
	{
	case eumDemod_11ag:
		tmpSignal = new Signal11ag;
		break;

	case eumDemod_11b:
		tmpSignal = new Signal11b;
		break;

	case eumDemod_11n_20M:
		tmpSignal = new Signal11n20m;
		break;

	case eumDemod_11n_40M:
		tmpSignal = new Signal11n40m;
		break;

	case eumDemod_11ac_20M:
		tmpSignal = new Signal11ac20m;
		break;

	case eumDemod_11ac_40M:
		tmpSignal = new Signal11ac40m;
		break;

	case eumDemod_11ac_80M:
		tmpSignal = new Signal11ac80m;
		break;

	case eumDemod_11ac_160M:
		tmpSignal = new Signal11ac160m;
		break;

	case eumDemod_11ac_80M_80M:
		tmpSignal = new Signal11ac80_80m;
		break;

	case eumDemod_11ax_20M:
		tmpSignal = new Signal11ax20m;
		break;

	case eumDemod_11ax_40M:
		tmpSignal = new Signal11ax40m;
		break;

	case eumDemod_11ax_80M:
		tmpSignal = new Signal11ax80m;
		break;

	case eumDemod_11ax_160M:
		tmpSignal = new Signal11ax160m;
		break;

	case eumDemod_11ax_80_80M:
		tmpSignal = new Signal11ax80_80m;
		break;

	case eumDemod_BT:
		tmpSignal = new SignalBT;
		break;

	case eumDemod_Zigbee:
		tmpSignal = new SignalZigbee;
		break;

	case eumDemod_GPS:
		tmpSignal = new SignalGPS;
		break;

	case eumDemod_BDS:
		tmpSignal = new SignalBDS;
		break;

	default:
		break;
	}

	return tmpSignal;
}

extern "C" SIGNAL_Decl void DeleteSignal( Signal *signal )
{
	delete dynamic_cast<SignalBase *>(signal);
}

extern "C" SIGNAL_Decl int ChangeDataRateToMcs( int demode, double dataRate )
{
	Signal *tmpSignal = CreateSignal(demode);

	int mcs = tmpSignal->ChangeDataRateToMcs(dataRate);

	DeleteSignal(tmpSignal);

	return mcs;
}

extern "C" SIGNAL_Decl double ChangeMcsToDataRate( int demode, int mcs )
{
	Signal *tmpSignal = CreateSignal(demode);

	double dataRate = tmpSignal->ChangeMcsToDataRate(mcs);

	DeleteSignal(tmpSignal);

	return dataRate;
}

extern "C" SIGNAL_Decl double ChangeChannelToFreqHz( int demode, int mcs, int channel )
{
	Signal *tmpSignal = CreateSignal(demode);
	tmpSignal->SetMcs(mcs);
	tmpSignal->SetChannel(channel);

	double freq = tmpSignal->GetFreqHz();

	DeleteSignal(tmpSignal);

	return freq;
}

extern "C" SIGNAL_Decl int ChangeFreqHzToChannel( int demode, int mcs, double FreqHz )
{
	Signal *tmpSignal = CreateSignal(demode);
	tmpSignal->SetMcs(mcs);
	tmpSignal->SetFreqHz(FreqHz);

	int channel = tmpSignal->GetChannel();

	DeleteSignal(tmpSignal);

	return channel;
}
