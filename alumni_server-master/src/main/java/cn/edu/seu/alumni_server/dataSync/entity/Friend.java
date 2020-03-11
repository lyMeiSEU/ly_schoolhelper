package cn.edu.seu.alumni_server.dataSync.entity;

import javax.persistence.Table;
import java.io.Serializable;
import java.util.Date;

@Table(name = "Friend")
public class Friend implements Serializable {
    private String openid;

    private String friendid;

    private String state;

    private Date time;

    private static final long serialVersionUID = 1L;

    /**
     * @return openid
     */
    public String getOpenid() {
        return openid;
    }

    /**
     * @param openid
     */
    public void setOpenid(String openid) {
        this.openid = openid == null ? null : openid.trim();
    }

    /**
     * @return friendid
     */
    public String getFriendid() {
        return friendid;
    }

    /**
     * @param friendid
     */
    public void setFriendid(String friendid) {
        this.friendid = friendid == null ? null : friendid.trim();
    }

    /**
     * @return state
     */
    public String getState() {
        return state;
    }

    /**
     * @param state
     */
    public void setState(String state) {
        this.state = state == null ? null : state.trim();
    }

    /**
     * @return time
     */
    public Date getTime() {
        return time;
    }

    /**
     * @param time
     */
    public void setTime(Date time) {
        this.time = time;
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append(getClass().getSimpleName());
        sb.append(" [");
        sb.append("Hash = ").append(hashCode());
        sb.append(", openid=").append(openid);
        sb.append(", friendid=").append(friendid);
        sb.append(", state=").append(state);
        sb.append(", time=").append(time);
        sb.append(", serialVersionUID=").append(serialVersionUID);
        sb.append("]");
        return sb.toString();
    }
}