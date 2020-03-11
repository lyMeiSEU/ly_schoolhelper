package cn.edu.seu.alumni_server.dataSync.entity;

import javax.persistence.Column;
import javax.persistence.Table;
import java.io.Serializable;

@Table(name = "Education")
public class Education implements Serializable {
    private String num;

    private String openid;

    private String school;

    private String background;

    private String department;

    private String profession;

    @Column(name = "start_year")
    private String startYear;

    @Column(name = "end_year")
    private String endYear;

    private static final long serialVersionUID = 1L;

    /**
     * @return num
     */
    public String getNum() {
        return num;
    }

    /**
     * @param num
     */
    public void setNum(String num) {
        this.num = num == null ? null : num.trim();
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
     * @return school
     */
    public String getSchool() {
        return school;
    }

    /**
     * @param school
     */
    public void setSchool(String school) {
        this.school = school == null ? null : school.trim();
    }

    /**
     * @return background
     */
    public String getBackground() {
        return background;
    }

    /**
     * @param background
     */
    public void setBackground(String background) {
        this.background = background == null ? null : background.trim();
    }

    /**
     * @return department
     */
    public String getDepartment() {
        return department;
    }

    /**
     * @param department
     */
    public void setDepartment(String department) {
        this.department = department == null ? null : department.trim();
    }

    /**
     * @return profession
     */
    public String getProfession() {
        return profession;
    }

    /**
     * @param profession
     */
    public void setProfession(String profession) {
        this.profession = profession == null ? null : profession.trim();
    }

    /**
     * @return start_year
     */
    public String getStartYear() {
        return startYear;
    }

    /**
     * @param startYear
     */
    public void setStartYear(String startYear) {
        this.startYear = startYear == null ? null : startYear.trim();
    }

    /**
     * @return end_year
     */
    public String getEndYear() {
        return endYear;
    }

    /**
     * @param endYear
     */
    public void setEndYear(String endYear) {
        this.endYear = endYear == null ? null : endYear.trim();
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append(getClass().getSimpleName());
        sb.append(" [");
        sb.append("Hash = ").append(hashCode());
        sb.append(", num=").append(num);
        sb.append(", openid=").append(openid);
        sb.append(", school=").append(school);
        sb.append(", background=").append(background);
        sb.append(", department=").append(department);
        sb.append(", profession=").append(profession);
        sb.append(", startYear=").append(startYear);
        sb.append(", endYear=").append(endYear);
        sb.append(", serialVersionUID=").append(serialVersionUID);
        sb.append("]");
        return sb.toString();
    }
}