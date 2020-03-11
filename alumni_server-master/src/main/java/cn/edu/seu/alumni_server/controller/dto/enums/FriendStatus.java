package cn.edu.seu.alumni_server.controller.dto.enums;


/**
 * 1. A,B 陌生人
 * <p>
 * 2.A向B发起好友请求
 * db里面会插入两条记录
 * A - B apply
 * B - A todo
 * <p>
 * 3.B 同意/忽略
 * - 同意
 * A - B friend
 * B - A friend
 * - 拒绝
 * A - B ignored
 * B - A stranger
 */
public enum FriendStatus {

    /**
     *
     */
    stranger(0, "路人"),
    apply(1, "A向B发起好友申请"),
    friend(2, "好友"),
    todo(3, "A收到B的好友请求"),
    ignored(4, "A的请求被忽略");

    private int status;
    private String desc;

    FriendStatus(int status, String desc) {
        this.status = status;
        this.desc = desc;
    }

    public int getStatus() {
        return status;
    }

    public String getDesc() {
        return desc;
    }
}
