package cn.edu.seu.alumni_server.dao.entity;


import lombok.Data;

import javax.persistence.Id;
import java.io.Serializable;
import java.util.Date;

@Data
public class AlumniCircle implements Serializable {

    @Id
    private Long alumniCircleId;

    private Long creatorId;

    private Integer alumniCircleType;

    private String alumniCircleName;

    private String alumniCircleDesc;
    private String alumniCircleAnnouncement;

    private String avatar;

    private Boolean authorizationStatus;

    private Date cTime;

    private Date uTime;

    private Boolean isAvailable;

    private static final long serialVersionUID = 1L;

}
