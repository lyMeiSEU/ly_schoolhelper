package cn.edu.seu.alumni_server.dataSync.entity;

import javax.persistence.Column;
import javax.persistence.Table;
import java.io.Serializable;

@Table(name = "PersonalInfor")

public class Personalinfor implements Serializable {
    @Column(name = "real_name")
    private String realName;

    private String gender;

    private String descr;

    private String vocation;

    private String city;

    private String openid;

    @Column(name = "head_url")
    private String headUrl;

    private String id;

    private String birth;

    private static final long serialVersionUID = 1L;

    /**
     * @return real_name
     */
    public String getRealName() {
        return realName;
    }

    /**
     * @param realName
     */
    public void setRealName(String realName) {
        this.realName = realName == null ? null : realName.trim();
    }

    /**
     * @return gender
     */
    public String getGender() {
        return gender;
    }

    /**
     * @param gender
     */
    public void setGender(String gender) {
        this.gender = gender == null ? null : gender.trim();
    }

    /**
     * @return descr
     */
    public String getDescr() {
        return descr;
    }

    /**
     * @param descr
     */
    public void setDescr(String descr) {
        this.descr = descr == null ? null : descr.trim();
    }

    /**
     * @return vocation
     */
    public String getVocation() {
        return vocation;
    }

    /**
     * @param vocation
     */
    public void setVocation(String vocation) {
        this.vocation = vocation == null ? null : vocation.trim();
    }

    /**
     * @return city
     */
    public String getCity() {
        return city;
    }

    /**
     * @param city
     */
    public void setCity(String city) {
        this.city = city == null ? null : city.trim();
    }

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
     * @return head_url
     */
    public String getHeadUrl() {
        return headUrl;
    }

    /**
     * @param headUrl
     */
    public void setHeadUrl(String headUrl) {
        this.headUrl = headUrl == null ? null : headUrl.trim();
    }

    /**
     * @return id
     */
    public String getId() {
        return id;
    }

    /**
     * @param id
     */
    public void setId(String id) {
        this.id = id == null ? null : id.trim();
    }

    /**
     * @return birth
     */
    public String getBirth() {
        return birth;
    }

    /**
     * @param birth
     */
    public void setBirth(String birth) {
        this.birth = birth == null ? null : birth.trim();
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append(getClass().getSimpleName());
        sb.append(" [");
        sb.append("Hash = ").append(hashCode());
        sb.append(", realName=").append(realName);
        sb.append(", gender=").append(gender);
        sb.append(", descr=").append(descr);
        sb.append(", vocation=").append(vocation);
        sb.append(", city=").append(city);
        sb.append(", openid=").append(openid);
        sb.append(", headUrl=").append(headUrl);
        sb.append(", id=").append(id);
        sb.append(", birth=").append(birth);
        sb.append(", serialVersionUID=").append(serialVersionUID);
        sb.append("]");
        return sb.toString();
    }
}