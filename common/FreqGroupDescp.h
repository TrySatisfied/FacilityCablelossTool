#pragma once

#include "Signal.h"
#include "common.h"

class COMMON_API cFreqGroupDescp
{
public:
	cFreqGroupDescp();
	virtual ~cFreqGroupDescp();

	bool bIsChannelValid(int nFreqGroup,int nChannel) const;
	int nGetFirstChannel(int nFreqGroup) const;
	int nGetLastChannel(int nFreqGroup) const;
	int nGetChannelCount(int nFreqGroup) const;
	int nGetNearlyValidChannel(int nFreqGroup,int nChannelRaw) const;

	int nGet5GSubChannelGroup_Start(int n5GChannelGroup) const;
	int nGet5GSubChannelGroup_End(int n5GChannelGroup) const;

	int nGet5GSubChannelGroup(int nChannel) const;
	int nGetIndexIn5GSubChannelGroup(int nChannel) const;

	int nGetChannelInValidChannelArrary(int nFreqGroup,int nIndex) const;
	int nGetIndexInValidChannelArray(int nFreqGroup,int nChannel) const;
	int nGetNextChannelInValidChannelValid(int nFreqGroup,int nChannel) const;

	bool bHasGroup_11AC() const {return m_hasGroup_11AC;};

protected:
	virtual void initValidChannel_2_4G();
	virtual void initValidChannel_5G();
	virtual void init5GSubChannelGroup();
	void initChannelMask();

	bool m_channelsMask[eumFreqGroupMax][eumChannelMax];
	int m_channelsCount[eumFreqGroupMax];

	std::vector<int> m_vectValidChannel_2_4G;
	std::vector<int> m_vectValidChannel_5G;

	std::vector<int> m_vect5GSubChannelGroup_Start;
	std::vector<int> m_vect5GSubChannelGroup_End;

	bool m_hasGroup_11AC;
};


//AC标准中添加了若干5G信道
class COMMON_API cFreqGroupDescp_11AC
	:public cFreqGroupDescp
{
public:
	cFreqGroupDescp_11AC();
	virtual ~cFreqGroupDescp_11AC(){};

protected:
	virtual void initValidChannel_5G();
	virtual void init5GSubChannelGroup();
};

