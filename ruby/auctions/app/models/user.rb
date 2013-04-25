class User < ActiveRecord::Base
  attr_accessible :email, :name, :password, :password_confirmation
  has_secure_password
  has_many :auctions, dependent: :destroy
  has_many :bids, dependent: :destroy

  before_save do |user|
    # User's email should be saved downcase to avoid duplications.
    user.email = email.downcase
    # Remember token is used to create a session
    user.remember_token = SecureRandom.urlsafe_base64
  end

  validates :name,
    presence: true,
    length: { maximum: 50 }
  
  validates :email,
    presence: true,
    length: { maximum: 50 },
    format: { with: /\A[\w+\-.]+@[a-z\d\-.]+\.[a-z]+\z/i },
    uniqueness: { case_sensitive: false }

  validates :password,
    presence: true

  validates :password_confirmation,
    presence: true

end
