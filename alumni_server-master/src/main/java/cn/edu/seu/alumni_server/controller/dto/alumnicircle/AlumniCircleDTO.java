package cn.edu.seu.alumni_server.controller.dto.alumnicircle;


import cn.edu.seu.alumni_server.dao.entity.AlumniCircle;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
import org.springframework.beans.BeanUtils;
import org.springframework.context.annotation.Bean;

@Data
@AllArgsConstructor
@NoArgsConstructor
public class AlumniCircleDTO {

    private Long alumniCircleId;

    private Long creatorId;

    // 群组的类型
    private Integer alumniCircleType;

    private String alumniCircleName;

    private String alumniCircleDesc;
    private String alumniCircleAnnouncement;

    // 群组的头像
    private String avatar;

    // 群组的验证状态
    private Boolean authorizationStatus;
    // 当前用户是否属于当前圈子
    private Boolean isJoined;

    public AlumniCircleDTO(AlumniCircle alumniCircle) {
        BeanUtils.copyProperties(alumniCircle, this);
    }

    public AlumniCircle toAlumniCircle() {
        AlumniCircle alumniCircle = new AlumniCircle();
        BeanUtils.copyProperties(this, alumniCircle);
        return alumniCircle;
    }
}
