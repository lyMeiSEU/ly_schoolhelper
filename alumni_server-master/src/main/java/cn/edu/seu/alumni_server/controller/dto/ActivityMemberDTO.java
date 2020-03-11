package cn.edu.seu.alumni_server.controller.dto;

import cn.edu.seu.alumni_server.dao.entity.ActivityMember;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
import org.springframework.beans.BeanUtils;


@Data
@NoArgsConstructor
@AllArgsConstructor
public class ActivityMemberDTO {
    private Long activityId;

    private Long accountId;

    private Boolean readStatus;

    public ActivityMemberDTO(ActivityMember activityMember) {
        BeanUtils.copyProperties(activityMember, this);
    }

    public ActivityMember toActivityMember() {
        ActivityMember activityMember = new ActivityMember();
        BeanUtils.copyProperties(this, activityMember);
        return activityMember;
    }
}
