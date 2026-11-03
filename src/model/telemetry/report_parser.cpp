// Model - 리포트(RPT) 수신 파서
//
// YonseiGS_coms.cpp 에서 리포트 패킷 해석 함수를 내용 그대로 옮긴 것.
// 540바이트 와이어 프레임 -> Report 구조체 (ParseReportWire540), Report -> 화면용 ReportView_t (UpdateReportViewFromReport).
// 두 함수는 다른 파일(task_downlink_onorbit)에서 부르므로 static 을 뗐다. 나머지 도우미는 이 파일 안에서만 쓴다.
#include "YonseiGS_config.h"
#include "YonseiGS_csp.h"
#include "YonseiGS_coms.h"
#include "YonseiGS_orbital.h"
#include "YonseiGS_radial.h"
#include <netinet/in.h>

#include <mutex>

#include "report_parser.h"

extern Console console;
extern StateCheckUnit State;

static uint16_t be16(const uint8_t *p) {
    return (uint16_t(p[0]) << 8) | uint16_t(p[1]);
}

static uint32_t be32u(const uint8_t *p) {
    return (uint32_t(p[0]) << 24) | (uint32_t(p[1]) << 16) | (uint32_t(p[2]) << 8) | uint32_t(p[3]);
}

static int32_t be32s(const uint8_t *p) {
    return (int32_t)be32u(p);
}

static bool DecodePayloadToView(ReportView_t &v, const uint8_t *payload, uint16_t payload_len) {
    if (!payload) return false;

    if (v.kind == REPORT_KIND_UANT_GET_STATUS_TLM) {
        if (payload_len < sizeof(gs_gssb_ant6_release_status_t)) {
            v.kind = REPORT_KIND_SC_GENERIC;
        } else {
            memcpy(&v.u.uant_getstatus, payload, sizeof(gs_gssb_ant6_release_status_t));
            return true;
        }
    }


    if (v.kind == REPORT_KIND_ADCS_LOG_MASK) {
        if (payload_len < sizeof(ADCS_TlmLogInclMaskTlm_Payload_t)) {
            v.kind = REPORT_KIND_SC_GENERIC;
        } else {
            memcpy(&v.u.adcs_logmask, payload, sizeof(ADCS_TlmLogInclMaskTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_UNSOLICIT_TLM_SETUP_TLM) {
        if (payload_len < sizeof(ADCS_UnsolicitTlmMsgSetupTlm_Payload_t)) {
            v.kind = REPORT_KIND_SC_GENERIC;
        } else {
            memcpy(&v.u.adcs_unsolicited_tlm_tlm, payload, sizeof(ADCS_UnsolicitTlmMsgSetupTlm_Payload_t));
            return true;
        }
    }


    if (v.kind == REPORT_KIND_EPS_P60_DOCK_GET_TABLE_HK) {
        if (payload_len < sizeof(EPS_P60_DOCK_GET_TABLE_HK)) {
            v.kind = REPORT_KIND_SC_GENERIC;
        } else {
            memcpy(&v.u.eps_p60dockgettablehk, payload, sizeof(EPS_P60_DOCK_GET_TABLE_HK));
            return true;
        }
    }


        if (v.kind == REPORT_KIND_EPS_P60_PDU_GET_TABLE_HK) {
        if (payload_len < sizeof(EPS_P60_PDU_GET_TABLE_HK)) {
            v.kind = REPORT_KIND_SC_GENERIC;
        } else {
            memcpy(&v.u.eps_p60pdugettablehk, payload, sizeof(EPS_P60_PDU_GET_TABLE_HK));
            return true;
        }
    }

        if (v.kind == REPORT_KIND_EPS_P60_ACU_GET_TABLE_HK) {
        if (payload_len < sizeof(EPS_P60_ACU_GET_TABLE_HK)) {
            v.kind = REPORT_KIND_SC_GENERIC;
        } else {
            memcpy(&v.u.eps_p60acugettablehk, payload, sizeof(EPS_P60_ACU_GET_TABLE_HK));
            return true;
        }
    }

            if (v.kind == REPORT_KIND_ADCS_RAW_RW_SENSOR_TLM) {
        if (payload_len < sizeof(ADCS2_RawRWLSensorTlm_Payload_t)) {
            v.kind = REPORT_KIND_SC_GENERIC;
        } else {
            memcpy(&v.u.adcs_raw_rw_sensor, payload, sizeof(ADCS2_RawRWLSensorTlm_Payload_t));
            return true;
        }
    }

            if (v.kind == REPORT_KIND_ADCS_RAW_GYR_SENSOR_TLM) {
        if (payload_len < sizeof(ADCS2_RawGYRSensorTlm_Payload_t)) {
            v.kind = REPORT_KIND_SC_GENERIC;
        } else {
            memcpy(&v.u.adcs_raw_gyr_sensor, payload, sizeof(ADCS2_RawGYRSensorTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_WHL_CONFIG)
    {
        if (payload_len < sizeof(ADCS_WhlConfigTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else
        {
            memcpy(&v.u.adcs_whlconfig, payload, sizeof(ADCS_WhlConfigTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_SATELLITE_CONFIG)
    {
        if (payload_len < sizeof(ADCS_SatelliteConfigTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else
        {
            memcpy(&v.u.adcs_satelliteconfig, payload, sizeof(ADCS_SatelliteConfigTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_CONTROLLER_CONFIG)
    {
        if (payload_len < sizeof(ADCS_ControllerConfigTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else
        {
            memcpy(&v.u.adcs_controllerconfig, payload, sizeof(ADCS_ControllerConfigTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_DEFAULT_MODE_CONFIG)
    {
        if (payload_len < sizeof(ADCS_DefaultModeConfigTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else
        {
            memcpy(&v.u.adcs_defaultmodeconfig, payload, sizeof(ADCS_DefaultModeConfigTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_MOUNTING_CONFIG)
    {
        if (payload_len < sizeof(ADCS_MountingConfigTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else
        {
            memcpy(&v.u.adcs_mountingconfig, payload, sizeof(ADCS_MountingConfigTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_ESTIMATOR_CONFIG)
    {
        if (payload_len < sizeof(ADCS_EstimatorConfigTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else
        {
            memcpy(&v.u.adcs_estimatorconfig, payload, sizeof(ADCS_EstimatorConfigTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_NODE_SELECTION_CONFIG)
    {
        if (payload_len < sizeof(ADCS_NodeSelectionConfigTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else
        {
            memcpy(&v.u.adcs_nodeselectionconfig, payload, sizeof(ADCS_NodeSelectionConfigTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_MTQ_CONFIG)
    {
        if (payload_len < sizeof(ADCS_NodeSelectionConfigTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else
        {
            memcpy(&v.u.adcs_mtqconfig, payload, sizeof(ADCS_MTQConfigTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_MAG_SENSING_ELM_CONFIG)
    {
        if (payload_len < sizeof(ADCS_MagSensingElmConfigTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else
        {
            memcpy(&v.u.adcs_magsensingelmconfig, payload, sizeof(ADCS_MagSensingElmConfigTlm_Payload_t));
            return true;
        }
    }

    if (v.kind == REPORT_KIND_ADCS_GET_SAT_ORBIT_PARAM_CONFIG)
    {
        if (payload_len < sizeof(ADCS_SatOrbitParamConfigTlm_Payload_t))
        {
            v.kind = REPORT_KIND_SC_GENERIC;
        }
        else
        {
            memcpy(&v.u.adcs_satorbitparamconfig, payload, sizeof(ADCS_SatOrbitParamConfigTlm_Payload_t));
            return true;
        }
    }

    v.kind = REPORT_KIND_SC_GENERIC;

    uint16_t n = payload_len;
    if (n > sizeof(v.u.generic.bytes)) n = sizeof(v.u.generic.bytes);
    memcpy(v.u.generic.bytes, payload, n);
    if (n < sizeof(v.u.generic.bytes)) memset(v.u.generic.bytes + n, 0, sizeof(v.u.generic.bytes) - n);

    return true;
}

bool ParseReportWire540(const uint8_t *buf, size_t len, Report &out) {
    if (!buf) return false;
    if (len < REPORT_WIRE_SIZE) return false;

    memset(&out, 0, sizeof(out));

    // ===== CCSDS-like header (16B assumed in your format) =====
    out.CCSDS_MsgId = be16(buf + 0);
    out.CCSDS_Seq   = be16(buf + 2);
    out.CCSDS_Len   = be16(buf + 4);
    memcpy(out.CCSDS_TimeCode, buf + 6, 6);
    out.CCSDS_Padding = be32u(buf + 12);

    // ===== Report body (starts at offset 16) =====
// ===== Report body (starts at offset 16) =====
    memcpy(&out.ReflectedMID, buf + 16, sizeof(out.ReflectedMID));
    memcpy(&out.ReflectedCC,  buf + 18, sizeof(out.ReflectedCC));
    memcpy(&out.RetType,      buf + 19, sizeof(out.RetType));
    memcpy(&out.RetCode,      buf + 20, sizeof(out.RetCode));
    memcpy(&out.RetValSize,   buf + 24, sizeof(out.RetValSize));

    // Return value bytes start at offset 26
    size_t copy_len = 512;
    if (out.RetValSize < copy_len) copy_len = out.RetValSize;
    memcpy(out.RetVal, buf + 26, copy_len);

    if (out.RetValSize > 512)
        out.RetValSize = 512;

    return true;
}

void UpdateReportViewFromReport(const Report &rpt) {
    ReportView_t tmp;
    memset(&tmp, 0, sizeof(tmp));

    tmp.valid = false;

    tmp.CCMessage_ID = rpt.CCSDS_MsgId;
    tmp.CCCount      = rpt.CCSDS_Seq;
    tmp.CCLength     = rpt.CCSDS_Len;
    memcpy(tmp.CCTime_code, rpt.CCSDS_TimeCode, 6);

    tmp.reflected_msg_id       = rpt.ReflectedMID;
    tmp.reflected_cc           = rpt.ReflectedCC;
    tmp.ret_type     = rpt.RetType;
    tmp.ret_code     = rpt.RetCode;

    tmp.ret_val_size = rpt.RetValSize;
    if (tmp.ret_val_size > 512) tmp.ret_val_size = 512;

    tmp.kind = DetermineBee1000ReportKind(tmp.reflected_msg_id, tmp.reflected_cc);

    DecodePayloadToView(tmp, rpt.RetVal, tmp.ret_val_size);

    tmp.valid = true;

    {
        std::lock_guard<std::mutex> lk(g_report_view_mtx);
        g_report_view = tmp;
    }
}
