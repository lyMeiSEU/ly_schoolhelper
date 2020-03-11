package cn.edu.seu.alumni_server.controller.dto.enums;

import org.apache.commons.lang3.EnumUtils;

public enum EventType {
    /**
     *
     */
    viewPost(0, "浏览帖子"),
    sharePost(1, "分享帖子");

    Integer value;
    String desc;

    EventType(Integer value, String desc) {
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