package cn.edu.seu.alumni_server.controller.dto.enums;

public enum MessageType {
    /**
     *
     */
    ACTIVITY_NOTIFY(10, "活动通知"),
    APPLY(0, "申请"),
    AGREE(1, "同意"),
    REJECT(2, "拒绝");

    Integer value;
    String desc;

    MessageType(Integer value, String desc) {
        this.value = value;
        this.desc = desc;
    }

    public Integer getValue() {
        return value;
    }

    public String getDesc() {
        return desc;
    }
}