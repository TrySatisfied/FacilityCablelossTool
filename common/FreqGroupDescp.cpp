#include "pch.h"
#include "FreqGroupDescp.h"

/*#####################################

            cFreqGroupDescp类

######################################*/
cFreqGroupDescp::cFreqGroupDescp()
{
    //子类重载以下任意函数时都需要在构造函数中重新调用
    initValidChannel_2_4G();
    initValidChannel_5G();
    init5GSubChannelGroup();

    //MARK:初始化VaildChannel和ChannelGroup后才能初始化ChannelMask
    //子类重载上面任意一个函数后，都需要在构造函数的最后重新初始化ChannelMask
    initChannelMask();
    m_hasGroup_11AC = false;
}

cFreqGroupDescp::~cFreqGroupDescp(void)
{
}

void cFreqGroupDescp::initValidChannel_2_4G()
{
	int narValidChannel_2_4G[]={
		1,2,3,4,5,6,7,8,9,10,11,12,13,14
	};
	m_vectValidChannel_2_4G.clear();
	for (int i=0;i<sizeof(narValidChannel_2_4G)/sizeof(narValidChannel_2_4G[0]);i++)
	{
		m_vectValidChannel_2_4G.push_back(narValidChannel_2_4G[i]);
	}
	return;
}

void cFreqGroupDescp::initValidChannel_5G()
{
	int narValidChannel_5G[]={
		36,38,40,44,46,48,50,52,54,56,58,60,62,64,
		100,102,104,106,108,110,112,116,118,120,122,124,126,128,130,132,134,136,138,140,142,144,
		146,149,151,153,157,159,161,163,165, 167, 169, 171, 173, 175, 177
	};
	m_vectValidChannel_5G.clear();
	for (int i=0;i<sizeof(narValidChannel_5G)/sizeof(narValidChannel_5G[0]);i++)
	{
		m_vectValidChannel_5G.push_back(narValidChannel_5G[i]);
	}
	return;
}

void cFreqGroupDescp::init5GSubChannelGroup()
{
    int nar5GSubChannelGroup_Start[] = {36, 100, 149};
    int nar5GSubChannelGroup_End[] = {64, 144, 177};

    m_vect5GSubChannelGroup_Start.clear();
    m_vect5GSubChannelGroup_End.clear();

    for (int i = 0; i < sizeof(nar5GSubChannelGroup_Start) / sizeof(nar5GSubChannelGroup_Start[0]); i++)
    {
        m_vect5GSubChannelGroup_Start.push_back(nar5GSubChannelGroup_Start[i]);
        m_vect5GSubChannelGroup_End.push_back(nar5GSubChannelGroup_End[i]);
    }

    return;
}

void cFreqGroupDescp::initChannelMask()
{
    for (int nFreqGroup = 0; nFreqGroup < eumFreqGroupMax; nFreqGroup++)
    {
        for (int iChannel = 0; iChannel < eumChannelMax; iChannel++)
        {
            m_channelsMask[nFreqGroup][iChannel] = false;
        }
    }

    int iEnableChannelIndex = 0;
    int nFreqGroup = eumFreqGroup2_4G;
    int iEnableChannelIndexMax = m_vectValidChannel_2_4G.size();
    m_channelsCount[nFreqGroup] = iEnableChannelIndexMax;

    for (int i = 0; i < eumChannelMax; i++)
    {
        if (i == m_vectValidChannel_2_4G[iEnableChannelIndex])
        {
            m_channelsMask[nFreqGroup][i] = true;
            iEnableChannelIndex++;

            if (iEnableChannelIndex >= iEnableChannelIndexMax)
            {
                break;
            }
        }
    }

    iEnableChannelIndex = 0;
    nFreqGroup = eumFreqGroup5G;
    iEnableChannelIndexMax = m_vectValidChannel_5G.size();
    m_channelsCount[nFreqGroup] = iEnableChannelIndexMax;

    for (int i = 0; i < eumChannelMax; i++)
    {
        if (i == m_vectValidChannel_5G[iEnableChannelIndex])
        {
            m_channelsMask[nFreqGroup][i] = true;
            iEnableChannelIndex++;

            if (iEnableChannelIndex >= iEnableChannelIndexMax)
            {
                break;
            }
        }
    }

    return;
}

bool cFreqGroupDescp::bIsChannelValid(int nFreqGroup, int nChannel) const
{
    return m_channelsMask[nFreqGroup][nChannel];
}

int cFreqGroupDescp::nGetFirstChannel(int nFreqGroup) const
{
    if (nFreqGroup == eumFreqGroup2_4G)
    {
        return m_vectValidChannel_2_4G[0];
    }

    return m_vectValidChannel_5G[0];
}

int cFreqGroupDescp::nGetLastChannel(int nFreqGroup) const
{
    if (nFreqGroup == eumFreqGroup2_4G)
    {
        return m_vectValidChannel_2_4G[m_channelsCount[eumFreqGroup2_4G] - 1];
    }

    return m_vectValidChannel_5G[m_channelsCount[eumFreqGroup5G] - 1];
}

int cFreqGroupDescp::nGetChannelCount(int nFreqGroup) const
{
    return m_channelsCount[nFreqGroup];
}

int cFreqGroupDescp::nGetNearlyValidChannel(int nFreqGroup, int nChannelRaw) const
{
    if (bIsChannelValid(nFreqGroup, nChannelRaw))
    {
        return nChannelRaw;
    }

    const std::vector<int> *pvectValidChannel;

    if ( nFreqGroup == eumFreqGroup2_4G)
    {
        pvectValidChannel = &m_vectValidChannel_2_4G;
    }
    else
    {
        pvectValidChannel = &m_vectValidChannel_5G;
    }

    bool bCompareResult = false;

    for (int nIndex = 0; nIndex < m_channelsCount[nFreqGroup]; nIndex++)
    {
        if (nChannelRaw <= pvectValidChannel->at(nIndex))
        {
            bCompareResult = true;
            nChannelRaw = pvectValidChannel->at(nIndex);
            break;
        }
    }

    if ( bCompareResult == false)
    {
        nChannelRaw = pvectValidChannel->at(m_channelsCount[nFreqGroup] - 1);
    }

    return nChannelRaw;
}

int cFreqGroupDescp::nGet5GSubChannelGroup_Start(int n5GChannelGroup) const
{
    return m_vect5GSubChannelGroup_Start[n5GChannelGroup];
}

int cFreqGroupDescp::nGet5GSubChannelGroup_End(int n5GChannelGroup) const
{
    return m_vect5GSubChannelGroup_End[n5GChannelGroup];
}

//返回-1时表示输入参数不属于任何5G Channel Group
int cFreqGroupDescp::nGet5GSubChannelGroup(int nChannel) const
{
    int n5GGroup = -1;

	for (int i = 0; i < eum5GSubChannelGroupMax; i++)
	{
		if ((nChannel >= m_vect5GSubChannelGroup_Start[i])
			&& (nChannel <= m_vect5GSubChannelGroup_End[i]))
		{
			n5GGroup = i;
			break;
		}
	}

    return n5GGroup;
}

//获取该channel在相应5G Channel Group中相应的Index(参考标准为m_iarValidChannel_5G数组)
int cFreqGroupDescp::nGetIndexIn5GSubChannelGroup(int nChannel) const
{
    int n5GGroup = nGet5GSubChannelGroup(nChannel);

    //输入参数不在各个5GChannelGroup之中
    if (n5GGroup < 0)
    {
        return -1;
    }

    int nChannelGroupStart = m_vect5GSubChannelGroup_Start[n5GGroup];
    int nStartIndex = -1;
    int nEndIndex = -1;

    for (int nChannelIndex = 0; nChannelIndex < m_channelsCount[eumFreqGroup5G]; nChannelIndex++)
    {
		if(nChannelGroupStart==nChannel)
		{
			return 0;
		}
        if (nChannelGroupStart == m_vectValidChannel_5G[nChannelIndex])
        {
            nStartIndex = nChannelIndex;
        }
        else if (nChannel == m_vectValidChannel_5G[nChannelIndex])
        {
            nEndIndex = nChannelIndex;
            break;
        }
    }

    //输入参数并非标准channel
	
    if ((nStartIndex == -1) || (nEndIndex == -1) || (nEndIndex < nStartIndex))
    {
        return -1;
    }

    return nEndIndex - nStartIndex;
}

int cFreqGroupDescp::nGetChannelInValidChannelArrary(int nFreqGroup, int nIndex) const
{
    if ( (nIndex < 0) || (nIndex >= m_channelsCount[nFreqGroup]))
    {
        return -1;
    }

    if ( nFreqGroup == eumFreqGroup2_4G)
    {
        return m_vectValidChannel_2_4G[nIndex];
    }
    else
    {
        return m_vectValidChannel_5G[nIndex];
    }
}

int cFreqGroupDescp::nGetIndexInValidChannelArray(int nFreqGroup, int nChannel) const
{
    const std::vector<int> *pvectValidChannelGroup;

    if (nFreqGroup == eumFreqGroup2_4G)
    {
        pvectValidChannelGroup = &m_vectValidChannel_2_4G;
    }
    else
    {
        pvectValidChannelGroup = &m_vectValidChannel_5G;
    }

    int nChannelCount = nGetChannelCount(nFreqGroup);

    for (int nChannelIndex = 0; nChannelIndex < nChannelCount; nChannelIndex++)
    {
        if (nChannel == pvectValidChannelGroup->at(nChannelIndex))
        {
            return nChannelIndex;
        }
    }

    return -1;
}

//返回输入参数channel在Valid Group中的下一个channel
int cFreqGroupDescp::nGetNextChannelInValidChannelValid(int nFreqGroup, int nChannel) const
{
    int nCurrentIndex = nGetIndexInValidChannelArray(nFreqGroup, nChannel);

    if (nCurrentIndex < 0)
    {
        return -1;
    }

    int nNextIndex = nCurrentIndex + 1;
    int nLastIndex = nGetChannelCount(nFreqGroup) - 1;

    if (nNextIndex > nLastIndex)
    {
        return -1;
    }

    return nGetChannelInValidChannelArrary(nFreqGroup, nNextIndex);
}



/*#####################################

            cFreqGroupDescp_AC类

######################################*/
cFreqGroupDescp_11AC::cFreqGroupDescp_11AC()
{
    initValidChannel_5G();
    init5GSubChannelGroup();

    initChannelMask();
    m_hasGroup_11AC = true;
}

void cFreqGroupDescp_11AC::initValidChannel_5G()
{
    int narValidChannel_5G[] =
    {
        36, 38, 40, 42, 44, 46, 48, 52, 54, 56, 58, 60, 62, 64,
        100, 102, 104, 106, 108, 110, 112, 116, 118, 120, 122,
        124, 126, 128, 132, 134, 136, 138, 140, 142, 144,
        149, 151, 153, 155, 157, 159, 161, 165, 167, 169, 171, 173, 175, 177
    };
    m_vectValidChannel_5G.clear();

    for (int i = 0; i < sizeof(narValidChannel_5G) / sizeof(narValidChannel_5G[0]); i++)
    {
        m_vectValidChannel_5G.push_back(narValidChannel_5G[i]);
    }

    return;
}

void cFreqGroupDescp_11AC::init5GSubChannelGroup()
{
    int nar5GSubChannelGroup_Start[] = {36, 100, 149};
    int nar5GSubChannelGroup_End[] = {64, 144, 177};
    m_vect5GSubChannelGroup_Start.clear();
    m_vect5GSubChannelGroup_End.clear();

    for (int i = 0; i < sizeof(nar5GSubChannelGroup_Start) / sizeof(nar5GSubChannelGroup_Start[0]); i++)
    {
        m_vect5GSubChannelGroup_Start.push_back(nar5GSubChannelGroup_Start[i]);
        m_vect5GSubChannelGroup_End.push_back(nar5GSubChannelGroup_End[i]);
    }

    return;
}

