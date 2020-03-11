package cn.edu.seu.alumni_server.controller.dto;

import cn.edu.seu.alumni_server.dao.entity.Friend;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
import org.springframework.beans.BeanUtils;

import java.text.SimpleDateFormat;
import java.util.Date;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class FriendDTO {
    private Long friendAccountId;
    private String name;
    private String company;
    private String position;
    private String avatar;
    private String city;
    private Long status;
    private String school;
    private String college;
    private String startTime;

    public void setStartTime(final String startTime){
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy");
        this.startTime = dateFormat.format(new Date(Long.parseLong(startTime)));
    }

    public FriendDTO(Friend friend) {
        BeanUtils.copyProperties(friend, this);
    }

    public Friend toFriend() {
        Friend friend = new Friend();
        BeanUtils.copyProperties(this, friend);
        return friend;
    }
}
