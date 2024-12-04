// STL headers
#include <iostream>
#include <iomanip>

// sbgCommonLib headers
#include <sbgCommon.h>
#include <network/sbgNetwork.h>

// sbgECom headers
#include <sbgEComLib.h>

// Project headers
#include <loggerManager/loggerEntry.h>
#include <loggerManager/loggerContext.h>

// Local headers
#include "loggerEntryGeneral.h"

namespace sbg
{

//----------------------------------------------------------------------//
//- CLoggerEntryUtcTime                                                -//
//----------------------------------------------------------------------//

std::string CLoggerEntryUtcTime::getName() const
{
	return "utcTime";
}

bool CLoggerEntryUtcTime::preProcess(CLoggerContext &context, const SbgEComLogUnion &logData)
{
	context.setUtcTime(logData.utcData);

	return true;
}

void CLoggerEntryUtcTime::writeHeaderToFile(const CLoggerContext &context)
{
	m_outFile	<< context.getTimeColTitle()	<< "\tstatus\tgpsTimeOfWeek\tyear\tmonth\tday\thour\tminute\tsecond\tnanosecond\tclkBiasStd\tclkSfErrorStd\tclkResidualError\n";
	m_outFile	<< context.getTimeUnit()		<< "\t(na)\t(ms)\t(yyyy)\t(mm)\t(dd)\t(hh)\t(mm)\t(s)\t(ns)\t(us)\t(ppm)\t(us)\n";
}

void CLoggerEntryUtcTime::writeDataToFile(const CLoggerContext &context, const SbgEComLogUnion &logData)
{
	const SbgEComLogUtc				&data = logData.utcData;

	m_outFile	<< context.fmtTime(data.timeStamp)										<< "\t"
				<< context.fmtStatus(data.status)										<< "\t"
				<< (uint32_t)data.gpsTimeOfWeek											<< "\t"
				<< (int32_t)data.year													<< "\t"
				<< (int32_t)data.month													<< "\t"
				<< (int32_t)data.day													<< "\t"
				<< (int32_t)data.hour													<< "\t"
				<< (int32_t)data.minute													<< "\t"
				<< (int32_t)data.second													<< "\t"
				<< (int32_t)data.nanoSecond												<< "\t"
				<< data.clkBiasStd*1e6f													<< "\t"
				<< data.clkSfErrorStd*1e9f												<< "\t"
				<< data.clkResidualError*1e6f											<< "\n";
}

void CLoggerEntryUtcTime::writeDataToConsole(const CLoggerContext &context, const SbgEComLogUnion &logData)
{
	const SbgEComLogUtc				&data = logData.utcData;

	std::cout	<< std::setw(12) << getName()											<< ": "
				<< std::setw(12) << context.fmtStatus(data.status)
				<< std::setw(12) << (uint32_t)data.gpsTimeOfWeek
				<< std::setw(12) << (int32_t)data.year
				<< std::setw(12) << (int32_t)data.month
				<< std::setw(12) << (int32_t)data.day
				<< std::setw(12) << (int32_t)data.hour
				<< std::setw(12) << (int32_t)data.minute
				<< std::setw(12) << (int32_t)data.second
				<< std::setw(12) << (int32_t)data.nanoSecond
				<< std::setw(12) << data.clkBiasStd*1e6f
				<< std::setw(12) << data.clkSfErrorStd*1e9f
				<< std::setw(12) << data.clkResidualError*1e6f							<< "\n";
}

//----------------------------------------------------------------------//
//- CLoggerEntryStatus                                                 -//
//----------------------------------------------------------------------//

std::string CLoggerEntryStatus::getName() const
{
	return "status";
}

void CLoggerEntryStatus::writeHeaderToFile(const CLoggerContext &context)
{
	m_outFile	<< context.getTimeColTitle()	<< "\tgeneral\tcom\tcom2\taiding\tcpuUsage\n";
	m_outFile	<< context.getTimeUnit()		<< "\t(na)\t(na)\t(na)\t(na)\t(na)\t(%%)\n";
}

void CLoggerEntryStatus::writeDataToFile(const CLoggerContext &context, const SbgEComLogUnion &logData)
{
	const SbgEComLogStatus				&data = logData.statusData;

	m_outFile	<< context.fmtTime(data.timeStamp)										<< "\t"
				<< context.fmtStatus(data.generalStatus)								<< "\t"
				<< context.fmtStatus(data.comStatus)									<< "\t"
				<< context.fmtStatus(data.comStatus2)									<< "\t"
				<< context.fmtStatus(data.aidingStatus)									<< "\t"
				<< (uint32_t)data.cpuUsage												<< "\n";
}

void CLoggerEntryStatus::writeDataToConsole(const CLoggerContext &context, const SbgEComLogUnion &logData)
{
	const SbgEComLogStatus				&data = logData.statusData;

	std::cout	<< std::setw(12) << getName()											<< ": "
				<< std::setw(12) << context.fmtStatus(data.generalStatus)
				<< std::setw(12) << context.fmtStatus(data.comStatus)
				<< std::setw(12) << context.fmtStatus(data.comStatus2)
				<< std::setw(12) << context.fmtStatus(data.aidingStatus)
				<< std::setw(12) << (uint32_t)data.cpuUsage								<< "\n";
}

//----------------------------------------------------------------------//
//- CLoggerEntryDiag                                                   -//
//----------------------------------------------------------------------//

std::string CLoggerEntryDiag::getName() const
{
	return "diag";
}

bool CLoggerEntryDiag::getDiscardData(const CLoggerContext &context) const
{
	SBG_UNUSED_PARAMETER(context);

	return false;
}

void CLoggerEntryDiag::writeDataToFile(const CLoggerContext &context, const SbgEComLogUnion &logData)
{
	const SbgEComLogDiagData		&data = logData.diagData;

	if (data.type == SBG_DEBUG_LOG_TYPE_ERROR)
	{
		m_outFile	<< context.fmtTime(data.timestamp)
					<< " *ERR * "
					<< data.string
					<< " [" << sbgErrorCodeToString(data.errorCode) << "]"
					<< std::endl;
	}
	else if (data.type == SBG_DEBUG_LOG_TYPE_WARNING)
	{
		m_outFile	<< context.fmtTime(data.timestamp)
					<< " *WARN * "
					<< data.string
					<< " [" << sbgErrorCodeToString(data.errorCode) << "]"
					<< std::endl;
	}
	else if (data.type == SBG_DEBUG_LOG_TYPE_INFO)
	{
		m_outFile	<< context.fmtTime(data.timestamp)
					<< " *INFO * "
					<< data.string
					<< std::endl;
	}
	else if (data.type == SBG_DEBUG_LOG_TYPE_DEBUG)
	{
		m_outFile	<< context.fmtTime(data.timestamp)
					<< " *DBG  * "
					<< data.string
					<< std::endl;
	}
	else
	{
		m_outFile	<< context.fmtTime(data.timestamp)
					<< " *UKN  * [" << sbgErrorCodeToString(data.errorCode) << "]: "
					<< data.string
					<< std::endl;
	}
}

void CLoggerEntryDiag::writeDataToConsole(const CLoggerContext &context, const SbgEComLogUnion &logData)
{
	const SbgEComLogDiagData		&data = logData.diagData;

	SBG_UNUSED_PARAMETER(context);

	if (data.type == SBG_DEBUG_LOG_TYPE_ERROR)
	{
		std::cout	<< " *ERR * "
					<< data.string
					<< " [" << sbgErrorCodeToString(data.errorCode) << "]"
					<< std::endl;
	}
	else if (data.type == SBG_DEBUG_LOG_TYPE_WARNING)
	{
		std::cout	<< " *WARN * "
					<< data.string
					<< " [" << sbgErrorCodeToString(data.errorCode) << "]"
					<< std::endl;
	}
}

//----------------------------------------------------------------------//
//- CLoggerEntryPtpStatus                                              -//
//----------------------------------------------------------------------//

std::string CLoggerEntryPtpStatus::convertState(SbgEComLogPtpState state)
{
	std::string							 stateStr = "unknown";

	switch (state)
	{
	case SBG_ECOM_LOG_PTP_STATE_DISABLED:
		stateStr = "disabled";
		break;
	case SBG_ECOM_LOG_PTP_STATE_FAULTY:
		stateStr = "faulty";
		break;
	case SBG_ECOM_LOG_PTP_STATE_MASTER:
		stateStr = "master";
		break;
	case SBG_ECOM_LOG_PTP_STATE_PASSIVE:
		stateStr = "passive";
		break;
	}

	return stateStr;
}

std::string CLoggerEntryPtpStatus::convertTimeScale(SbgEComLogPtpTimeScale timeScale)
{
	std::string							 timeScaleStr = "unknown";

	switch (timeScale)
	{
	case SBG_ECOM_LOG_PTP_TIME_SCALE_TAI:
		timeScaleStr = "tai";
		break;
	case SBG_ECOM_LOG_PTP_TIME_SCALE_UTC:
		timeScaleStr = "utc";
		break;
	case SBG_ECOM_LOG_PTP_TIME_SCALE_GPS:
		timeScaleStr = "gps";
		break;
	}

	return timeScaleStr;
}

std::string CLoggerEntryPtpStatus::getName() const
{
	return "ptpStatus";
}

void CLoggerEntryPtpStatus::writeHeaderToFile(const CLoggerContext &context)
{
	m_outFile	<< context.getTimeColTitle()	<< "\tstate\ttimeScale\ttimeScaleOffset\tlocalClockIdentity\tlocalClockPriority1\tlocalClockPriority2\tlocalClockClass\tlocalClockAccuracy\tlocalClockLog2Variance\tlocalClockTimeSource\tmasterClockIdentity\tmasterClockPriority1\tmasterClockPriority2\tmasterClockClass\tmasterClockAccuracy\tmasterClockLog2Variance\tmasterClockTimeSource\tmasterIpAddress\tmeanPathDelay\tmeanPathDelayStdDev\tclockOffset\tclockOffsetStdDev\tclockFreqOffset\tclockFreqOffsetStdDev\n";
	m_outFile	<< context.getTimeUnit()		<< "\t(na)\t(na)\t(s)\t(na)\t(na)\t(na)\t(na)\t(na)\t(na)\t(na)\t(na)\t(na)\t(na)\t(na)\t(na)\t(na)\t(na)\t(na)\t(s)\t(s)\t(s)\t(s)\t(s)\t(s)\n";
}

void CLoggerEntryPtpStatus::writeDataToFile(const CLoggerContext &context, const SbgEComLogUnion &logData)
{
	const SbgEComLogPtp				&data = logData.ptpData;
	char							 masterIpAddressString[SBG_NETWORK_IPV4_STRING_SIZE];

	sbgNetworkIpToString(data.masterIpAddress, masterIpAddressString, sizeof(masterIpAddressString));

	m_outFile	<< context.fmtTime(data.timeStamp)										<< "\t"
				<< convertState(data.state)												<< "\t"
				<< convertTimeScale(data.timeScale)										<< "\t"
				<< data.timeScaleOffset													<< "\t"

				<< std::hex << data.localClockIdentity << std::dec						<< "\t"
				<< (uint32_t)data.localClockPriority1									<< "\t"
				<< (uint32_t)data.localClockPriority2									<< "\t"
				<< (uint32_t)data.localClockClass										<< "\t"
				<< (uint32_t)data.localClockAccuracy									<< "\t"
				<< data.localClockLog2Variance											<< "\t"
				<< (uint32_t)data.localClockTimeSource									<< "\t"

				<< std::hex << data.masterClockIdentity << std::dec						<< "\t"
				<< (uint32_t)data.masterClockPriority1									<< "\t"
				<< (uint32_t)data.masterClockPriority2									<< "\t"
				<< (uint32_t)data.masterClockClass										<< "\t"
				<< (uint32_t)data.masterClockAccuracy									<< "\t"
				<< data.masterClockLog2Variance											<< "\t"
				<< (uint32_t)data.masterClockTimeSource									<< "\t"
				<< masterIpAddressString												<< "\t"

				<< data.meanPathDelay													<< "\t"
				<< data.meanPathDelayStdDev												<< "\t"
				<< data.clockOffset														<< "\t"
				<< data.clockOffsetStdDev												<< "\t"
				<< data.clockFreqOffset													<< "\t"
				<< data.clockFreqOffsetStdDev											<< "\n";
}

void CLoggerEntryPtpStatus::writeDataToConsole(const CLoggerContext &context, const SbgEComLogUnion &logData)
{
	const SbgEComLogPtp				&data = logData.ptpData;
	char							 masterIpAddressString[SBG_NETWORK_IPV4_STRING_SIZE];

	SBG_UNUSED_PARAMETER(context);

	sbgNetworkIpToString(data.masterIpAddress, masterIpAddressString, sizeof(masterIpAddressString));

	std::cout	<< std::setw(12) << getName()											<< ": "
				<< std::setw(12) << convertState(data.state)
				<< std::setw(12) << convertTimeScale(data.timeScale)
				<< std::setw(12) << data.timeScaleOffset

				<< std::setw(20) << std::hex << data.localClockIdentity << std::dec
				<< std::setw(20) << std::hex << data.masterClockIdentity << std::dec
				<< std::setw(20) << masterIpAddressString

				<< std::setw(12) << data.meanPathDelay
				<< std::setw(12) << data.meanPathDelayStdDev
				<< std::setw(12) << data.clockOffset
				<< std::setw(12) << data.clockOffsetStdDev
				<< std::setw(12) << data.clockFreqOffset
				<< std::setw(12) << data.clockFreqOffsetStdDev							<< "\n";
}

//----------------------------------------------------------------------//
//- CLoggerEntryRtcmRaw                                                -//
//----------------------------------------------------------------------//

std::string CLoggerEntryRtcmRaw::getName() const
{
	return "rtcm";
}

std::string CLoggerEntryRtcmRaw::getFileName() const
{
	return getName() + ".bin";
}

bool CLoggerEntryRtcmRaw::isBinary() const
{
	return true;
}

void CLoggerEntryRtcmRaw::writeDataToFile(const CLoggerContext &context, const SbgEComLogUnion &logData)
{
	SBG_UNUSED_PARAMETER(context);

	m_outFile.write(reinterpret_cast<const char*>(logData.rtcmRawData.rawBuffer), logData.rtcmRawData.bufferSize);
}

}; // Namespace sbg
