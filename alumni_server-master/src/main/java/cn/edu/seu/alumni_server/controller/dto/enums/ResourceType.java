package cn.edu.seu.alumni_server.controller.dto.enums;

public enum ResourceType {

    post(1, "帖子"),
    comment(2, "评论");

    private int value;
    private String desc;

    ResourceType(int value, String desc){
        this.value = value;
        this.desc = desc;
    }

    int getValue(){
        return value;
    }

    String getDesc(){
        return desc;
    }

}
