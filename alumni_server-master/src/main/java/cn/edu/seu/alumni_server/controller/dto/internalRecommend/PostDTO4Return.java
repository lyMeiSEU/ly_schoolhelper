package cn.edu.seu.alumni_server.controller.dto.internalRecommend;

import cn.edu.seu.alumni_server.dao.entity.Post;
import lombok.Data;
import org.springframework.beans.BeanUtils;

@Data
public class PostDTO4Return {
    private Long accountId;
    private Long postId;
    private Integer type;
    private String jobName;
    private String industry;
    private String companyName;
    private String province;
    private Integer city;
    private Integer salaryRange;
    private String jobDetail;
    private Long cTime;
    private String img1;
    private String img2;
    private String img3;
    private String img4;
    private String img5;
    private String img6;

    private String viewCount;
    private String commentCount;

    private String poster_name;
    private String poster_avatar;
    private String poster_compony;
    private String poster_position;

    public PostDTO4Return() {
    }

    public PostDTO4Return(Post post) {
        BeanUtils.copyProperties(post, this);
    }

    public Post toPost() {
        Post post = new Post();
        BeanUtils.copyProperties(this, post);
        return post;
    }
}
