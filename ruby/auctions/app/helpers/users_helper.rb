module UsersHelper

  def gravatar_of(user)
    gid = Digest::MD5::hexdigest(user.email.downcase)
    image_tag "https://secure.gravatar.com/avatar/#{gid}",
      alt: user.name,
      class: 'gravatar' 
  end

end
